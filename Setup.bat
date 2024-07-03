@echo off
@chcp 1252 >nul
color a
setlocal

@rem arduino-cli lib install "LoRa"
@rem arduino-cli lib install "Ultrasonic"
@rem arduino-cli compile --fqbn arduino:avr:uno <caminho>
@rem arduino-cli board list
@rem arduino-cli upload -p <porta> --fqbn arduino:avr:uno <caminho>

@rem ---------------------------------------------------------------------------------------

@rem porta sender
set SENDER_PORT=COM5

@rem porta receiver
set RECEIVER_PORT=COM6

@rem caminho Sender
set SENDER_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Sender\Sender.ino"

@rem caminho Receiver
set RECEIVER_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Receiver\Receiver.ino"

@rem ---------------------------------------------------------------------------------------

echo.
echo  -----------
echo  - COMPILE -
echo  -----------
echo.

@rem compile e upload do Sender
arduino-cli compile --fqbn arduino:avr:uno %SENDER_PATH%

echo. 
echo ---------------------------------------------------------------------------------------
echo. 
echo  ----------
echo  - UPLOAD -
echo  ----------
echo. 

arduino-cli upload -p %SENDER_PORT% --fqbn arduino:avr:uno %SENDER_PATH%

echo.
echo ---------------------------------------------------------------------------------------
echo ---------------------------------------------------------------------------------------
echo.
echo  -----------
echo  - COMPILE -
echo  -----------
echo.

arduino-cli compile --fqbn arduino:avr:uno %RECEIVER_PATH%

echo.
echo ---------------------------------------------------------------------------------------
echo. 
echo  ----------
echo  - UPLOAD -
echo  ----------
echo.

arduino-cli upload -p %RECEIVER_PORT% --fqbn arduino:avr:uno %RECEIVER_PATH%

endlocal
pause >nul