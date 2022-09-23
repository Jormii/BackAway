# Remove old files
TO_REMOVE="*.PBP *.SFO *.elf *.prx Makefile"
for NAME in $TO_REMOVE; do
    FILES=$(find . -name $NAME)
    if [[ $FILES ]]; then
        rm $FILES
    fi
done

# Compile
python ./makefile_script.py
if [ $? -eq 0 ]; then
    make -B 1>/dev/null

    # Delete .o files
    DIRS=".."
    for DIR in $DIRS; do
        OBJ_FILES=$(find $DIR -name *.o)
        if [[ $OBJ_FILES ]]; then
            rm $OBJ_FILES
        fi
    done
fi
