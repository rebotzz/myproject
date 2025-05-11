if(Test-Path bin/shooter.exe)
{
    rm bin/shooter.exe
}

cd SDLShooter
make all
mv shooter.exe ../bin
cd ..