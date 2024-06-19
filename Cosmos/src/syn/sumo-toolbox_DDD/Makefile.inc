# If the MATLABDIR environment variable is not set
# set it to some default value
#
# Note note that the JAVA_HOME environment variable is assumed to be set
# to the path of your Java SDK installation

MATLABDIR ?= /usr/local/MATLAB/2014a/

export VERSION=2014a
export DATE=$(shell date "+%Y-%m-%d")
export TIME=$(shell date "+%H:%M")

export HOMEPAGE=http://sumo.intec.ugent.be
export WIKIPAGE=http://sumowiki.intec.ugent.be
export SUMOICON=sumo.ico

export COMMIT=custom
