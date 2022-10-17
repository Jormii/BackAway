import os
import zipfile
from xml.etree.ElementTree import ElementTree

import numpy as np

SCALE = 30
INPUT_DIR = "./assets/"
OUTPUT_DIR = "./compilation/resources/levels/"
EXTENSION = ".ggb"


class Level:

    # Y coordinate is negated
    FLIP = np.array([1, -1], dtype="float32")

    class Goal:

        def __init__(self, segment):
            self.segment = SCALE * Level.FLIP * segment
            self.segment = self.segment[np.argsort(self.segment[:, 1])]

    class Wall:

        def __init__(self, polyline, ephemeral):
            self.polyline = SCALE * Level.FLIP * polyline
            self.ephemeral = ephemeral

    def __init__(self, spawn, goal, objectives, walls):
        self.spawn = SCALE * Level.FLIP * spawn
        self.goal = goal
        self.objectives = SCALE * Level.FLIP * objectives
        self.walls = walls

    def write(self, path, output_dir):
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        _, filename = os.path.split(path)
        filename_no_ext, _ = os.path.splitext(filename)
        output_path = os.path.join(output_dir, f"{filename_no_ext}.level")
        with open(output_path, "wb") as fd:
            # Spawn
            fd.write(self.spawn.tobytes())

            # Goal
            fd.write(self.goal.segment[0, 0].tobytes())
            fd.write(self.goal.segment[0, 1].tobytes())
            fd.write(self.goal.segment[1, 1].tobytes())

            # Objectives
            n_objectives = np.array(len(self.objectives), dtype="uint32")
            fd.write(n_objectives.tobytes())
            fd.write(self.objectives.tobytes())

            # Walls
            n_walls = np.array(len(self.walls), dtype="uint32")
            fd.write(n_walls.tobytes())
            for wall in self.walls:
                fd.write(np.array(wall.ephemeral, dtype="uint32").tobytes())
                fd.write(np.array(len(wall.polyline), dtype="uint32").tobytes())
                fd.write(wall.polyline.tobytes())


def parse_xml(path):
    et = ElementTree()
    tree = et.parse(path)

    # Points
    points = {}
    complex = {}
    elements = tree.findall(".//element")
    for e in elements:
        t = e.attrib.get("type")
        if t != "point":
            continue

        label = e.attrib["label"]
        coords = e.find("./coords")
        coord_style = e.find("./coordStyle")
        if coord_style is None:
            tgt_dict = points
        else:
            tgt_dict = complex

        tgt_dict[label] = np.array(
            [coords.attrib["x"], coords.attrib["y"]], dtype="float32")

    # Objectives
    objectives = []
    for label, objective in complex.items():
        if label != "SPAWN":
            objectives.append(objective)

    if len(objectives) != 0:
        objectives = np.array(objectives)
    else:
        objectives = np.zeros((0, 2), dtype="float32")

    # Polylines
    segments = []
    polylines = []
    commands = tree.findall(".//command")
    for c in commands:
        name = c.attrib.get("name")

        tgt_list = None
        if name == "Segment":
            tgt_list = segments
        elif name == "PolyLine":
            tgt_list = polylines

        if tgt_list is None:
            continue

        segment = []
        vertices = c.find("./input")
        for label in vertices.attrib.values():  # TODO: Are in order?
            segment.append(points[label])

        tgt_list.append(np.array(segment, dtype="float32"))

    # Validate value
    assert "SPAWN" in complex, f"{path}: No spawn"
    assert len(polylines) != 0, f"{path}: No walls"
    assert len(segments) == 1, f"{path}: No goal or multiple goals"
    assert segments[0][0, 0] == segments[0][1, 0], \
        f"{path}: Goal X coordinate doesn't match"

    # Add bounding polyline
    min_corner = polylines[0][0]
    max_corner = polylines[0][0]
    for polyline in polylines:
        local_min = np.min(polyline, axis=0)
        local_max = np.max(polyline, axis=0)
        min_corner = np.minimum(min_corner, local_min)
        max_corner = np.maximum(max_corner, local_max)

    min_corner -= 1
    max_corner += 1
    bounding_dim = max_corner - min_corner
    bounding_polyline = np.array([
        min_corner,
        min_corner + (bounding_dim[0], 0),
        max_corner,
        min_corner + (0, bounding_dim[1])
    ], dtype="float32")
    polylines.append(bounding_polyline)

    # Format and return level
    spawn = complex["SPAWN"]
    goal = Level.Goal(segments[0])
    walls = [Level.Wall(p, False) for p in polylines]
    level = Level(spawn, goal, objectives, walls)

    # TODO: Remove
    import matplotlib.pyplot as plt

    plt.plot(spawn[0], spawn[1], "gx")
    plt.plot(objectives[:, 0], objectives[:, 1], "g+")
    for polyline in polylines:
        plt.plot(polyline[:, 0], polyline[:, 1], "r-")
    for segment in segments:
        plt.plot(segment[:, 0], segment[:, 1], "b-")

    plt.gca().set_aspect("equal")
    plt.show()
    plt.close()

    return level


def parse_level(path, output_dir):
    with zipfile.ZipFile(path, "r") as fd:
        fd.extract("geogebra.xml")
        level = parse_xml("geogebra.xml")
        level.write(path, output_dir)
        os.remove("geogebra.xml")


def main():
    for filename in os.listdir(INPUT_DIR):
        _, extension = os.path.splitext(filename)
        if extension == EXTENSION:
            path = os.path.join(INPUT_DIR, filename)
            parse_level(path, OUTPUT_DIR)


if __name__ == "__main__":
    main()
