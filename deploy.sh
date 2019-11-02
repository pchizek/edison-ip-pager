#!/bin/sh
# Complile and deploy

# deploy (-d) NAME FILE

# arguments
# -d, --debug: debug
# 

# Get arguments

# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
VERBOSE=" "
DEBUG=" "
NEWNAME=0
NEWSOURCE=0
NEWDEPLOY=0

#  PARSE OPTIONS
while getopts "h,v,g,n:,s:,d:" opt; do
    case "$opt" in
    
    # help
    h)
        echo "help"
        exit 0
        ;;

    
    # Verbose
    v)  VERBOSE="-v "
        ;;

    # Debug
    g)
        DEBUG=" -g "
        ;;

    # Name
    n) 
        BUILDNAME="$OPTARG"
        
        source exportenv.sh EDISONBUILDNAME $BUILDNAME

        NEWNAME="1"
        
        ;;

    # Source
    s) 

        SOURCEPATH="$OPTARG"
        
        source exportenv.sh EDISONSOURCEPATH $SOURCEPATH
        
        NEWSOURCE=1

        ;;

    # Destination on Edison
    d)  
    
        DEPLOYPATH=$OPTARG

        source exportenv.sh EDISONDEPLOYPATH $DEPLOYPATH

        NEWDEPLOY="1"

        ;;

    \?) 
        echo "Invalid option: -$OPTARG" 
        exit 1
        ;;
    esac

done

# Errors for not having these params
: "${EDISONBUILDNAME:?Build name not defined.}"
: "${EDISONSOURCEPATH:?Source path not defined.}"
: "${EDISONDEPLOYPATH:?Deployment path not defined.}"

# Echo the given parameters
if [ $NEWNAME = "0" ]; then
    echo "Using previously specified build name: $EDISONBUILDNAME"
    BUILDNAME = "$EDISONBUILDNAME"    

fi

if [ $NEWSOURCE = "0" ]; then
    echo "Using previously specified source path: $EDISONSOURCEPATH"
    DEPLOYPATH = "$EDISONSOURCEPATH"  
fi

echo "Compiling..."

# Complile
eval "cc $VERBOSE-m32$DEBUG-o $BUILDNAME $SOURCEPATH"

#  Deploy

# Get IP
if [ -v "EDISONIP" ]; then
    echo "Enter IP Address of Edison (Previous: $EDISONIP)"
else
    echo "Enter IP Address of Edison."
fi

read INPUTIP

ARGSTRING="put ${EDISONBUILDNAME}"

if [ -z "$INPUTIP" ]; then 

    : "${EDISONIP:?IP address not found.}"
    INPUTIP=$EDISONIP

else 

    source exportenv.sh EDISONIP $INPUTIP

fi

sftp root@$INPUTIP <<< "$ARGSTRING"

# Complete
echo "Upload complete."
