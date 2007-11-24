# VIPER Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h

[ BLOCK <1> rel2 cprel2 ]
#========================
    <CPCMD> viper.doc            <RELROOT>/ide.doc
    <CPCMD> viper/ide.cfg        <RELROOT>/binw/ide.cfg
    <CPCMD> viper/idedos.cfg     <RELROOT>/binw/idedos.cfg
    <CPCMD> viper/idedos32.cfg   <RELROOT>/binw/idedos32.cfg
    <CPCMD> viper/idewin.cfg     <RELROOT>/binw/idewin.cfg
    <CPCMD> viper/ideos2.cfg     <RELROOT>/binw/ideos2.cfg
    <CPCMD> viper/ideos232.cfg   <RELROOT>/binw/ideos232.cfg
    <CPCMD> viper/idew32.cfg     <RELROOT>/binw/idew32.cfg
    <CPCMD> viper/idew386.cfg    <RELROOT>/binw/idew386.cfg
#    <CPCMD> viper/ideads.cfg     <RELROOT>/binw/ideads.cfg
    <CPCMD> viper/idenlm.cfg     <RELROOT>/binw/idenlm.cfg
#    <CPCMD> viper/idemfc16.cfg   <RELROOT>/binw/idemfc16.cfg
#    <CPCMD> viper/idemfc32.cfg   <RELROOT>/binw/idemfc32.cfg
#    <CPCMD> viper/idemfca.cfg    <RELROOT>/binw/idemfca.cfg
    <CPCMD> viper/ideaxp.cfg     <RELROOT>/binw/ideaxp.cfg
    <CPCMD> viper/idelnx.cfg     <RELROOT>/binw/idelnx.cfg

    <CPCMD> viper/ideunix.cfg    <RELROOT>/binl/ide.cfg
    <CPCMD> viper/idedos.cfg     <RELROOT>/binl/idedos.cfg
    <CPCMD> viper/idedos32.cfg   <RELROOT>/binl/idedos32.cfg
    <CPCMD> viper/idewin.cfg     <RELROOT>/binl/idewin.cfg
    <CPCMD> viper/ideos2.cfg     <RELROOT>/binl/ideos2.cfg
    <CPCMD> viper/ideos232.cfg   <RELROOT>/binl/ideos232.cfg
    <CPCMD> viper/idew32.cfg     <RELROOT>/binl/idew32.cfg
    <CPCMD> viper/idew386.cfg    <RELROOT>/binl/idew386.cfg
    <CPCMD> viper/idenlm.cfg     <RELROOT>/binl/idenlm.cfg
    <CPCMD> viper/ideaxp.cfg     <RELROOT>/binl/ideaxp.cfg
    <CPCMD> viper/idelnx.cfg     <RELROOT>/binl/idelnx.cfg

  [ IFDEF (os_win "") <2*> ]
    <CPCMD> viper/wini86/ide.exe <RELROOT>/binw/ide.exe
    <CPCMD> viper/win/idex.cfg   <RELROOT>/binw/idex.cfg
    <CPCMD> viper/win/wsrv.pif   <RELROOT>/binw/wsrv.pif
    <CPCMD> viper/win/wd.pif     <RELROOT>/binw/wd.pif

  [ IFDEF (os_os2 "") <2*> ]
    <CPCMD> viper/os2386/ide.exe <RELROOT>/binp/ide.exe
    <CPCMD> viper/os2/idex.cfg   <RELROOT>/binp/idex.cfg

  [ IFDEF (os_nt "") <2*> ]
    <CPCMD> viper/nt386/ide.exe  <RELROOT>/binnt/ide.exe
    <CPCMD> viper/nt/idex.cfg    <RELROOT>/binnt/idex.cfg

  [ IFDEF (cpu_axp) <2*> ] 
    <CPCMD> viper/ntaxp/ide.exe  <RELROOT>/axpnt/ide.exe
    <CPCMD> viper/axp/idex.cfg   <RELROOT>/axpnt/idex.cfg

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>
