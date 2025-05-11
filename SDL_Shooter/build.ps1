if(Test-Path bin/shooter.exe)
{
    rm bin/shooter.exe
}

cd SDLShooter
make all -f .\myMakefile
mv shooter.exe ../bin
cd ..