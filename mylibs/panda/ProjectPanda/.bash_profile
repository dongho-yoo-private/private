# .bash_profile

# Get the aliases and functions
if [ -f ~/.bashrc ]; then
	. ~/.bashrc
fi

# User specific environment and startup programs

PATH=$PATH:$HOME/bin

export PATH

export PANDA_PROJECT_TOP=$HOME/myproject
export PANDA_LIB_DIR=$PANDA_PROJECT_TOP/lib
export PANDA_INCLUDE_DIR=$PANDA_PROJECT_TOP/include
export PANDA_BIN_DIR=$PANDA_PROJECT_TOP/bin
export WSX_DIR_TOP=$PANDA_PROJECT_TOP/ProjectWosix
export WSX_OBJ_DIR=$WSX_DIR_TOP/objs

# Yotsuya Dir
export YOTSUYA_SVRDEF_DIR=/var/soft_yotsuya/svrdef
export YOTSUYA_LOCAL_PORT_DIR=/var/soft_yotsuya/localports
export YOTSUYA_USR_DIR=/var/soft_yotsuya/user
export YOTSUYA_SVRDATA_DIR=/var/soft_yotsuya/svrdata
export YOTSUYA_HOME=$HOME/usr/local/yotsuya

export CVSROOT=/home/panda/cvsroot

ulimit -c unlimited


