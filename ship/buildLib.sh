#!/bin/bash
#
# Creates one static library from several in current directory, lib is named $(LIBNAME)
#
# Usage: copy all your libs to a single directory and call this script.
#

# Inputs
LIBNAME=libscripal.a
LIBSDIR=./
# Tmp dir
OBJDIR=/tmp/unify-static-libs
mkdir -p ${OBJDIR}

# remove old library
rm -f $(LIBNAME)

# Extract .o
echo "Extracting objects to ${OBJDIR}..."
for i in ${LIBSDIR}/*.a
do
    echo $i
    ar --output $OBJDIR -x $i
done
# Link objects into a single lib
echo "Creating $LIBNAME from objects..."
ar -crs $LIBNAME $OBJDIR/*.o
# Clean up
rm -rf ${OBJDIR}
echo "Done."
