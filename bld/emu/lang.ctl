# EMU Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <LANG_BLD>/master.ctl ]
[ INCLUDE <LANG_BLD>/wproj.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h
    cd <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> <DEVDIR>/emu/dos386/emu387.lib  <RELROOT>/rel2/lib386/dos/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/netware/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/os2/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/win/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/qnx/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/nt/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/linux/emu387.lib
    <CPCMD> <DEVDIR>/emu/stub386/emu387.lib <RELROOT>/rel2/lib386/noemu387.lib
#    <CPCMD> <DEVDIR>/emu/qnx386/emu387      <RELROOT>/rel2/qnx/binq/emu387

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean
