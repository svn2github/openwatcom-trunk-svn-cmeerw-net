#pmake: build debug

all : .SYMBOLIC
    @echo Only "clean" is allowed at this level

clean : .SYMBOLIC
    cd dos
    wmake -h clean
    cd ../htmlhelp
    wmake -h clean
    cd ../nt
    wmake -h clean
    cd ../os2
    wmake -h clean
    cd ../pdf
    wmake -h clean
    cd ../win
    wmake -h clean
    cd ..

