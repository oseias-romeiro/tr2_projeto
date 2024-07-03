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

@rem porta NODE
set NODE_PORT=COM6

@rem porta GATEWAY
set GATEWAY_PORT=COM5

@rem caminho NODE
set NODE_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Node\Node.ino"

@rem caminho GATEWAY
set GATEWAY_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Gateway\Gateway.ino"

@rem ---------------------------------------------------------------------------------------

echo ---------------------------------------------------------------------------------------
echo  NODE 
echo ---------------------------------------------------------------------------------------
echo.

echo  -----------
echo  - COMPILE -
echo  -----------
echo.

@rem compile e upload do NODE
arduino-cli compile --fqbn arduino:avr:uno %NODE_PATH%

echo. 
echo  ----------
echo  - UPLOAD -
echo  ----------
echo. 

arduino-cli upload -p %NODE_PORT% --fqbn arduino:avr:uno %NODE_PATH%

echo.
echo ---------------------------------------------------------------------------------------
echo  GATEWAY
echo ---------------------------------------------------------------------------------------

echo.
echo  -----------
echo  - COMPILE -
echo  -----------
echo.

arduino-cli compile --fqbn arduino:avr:uno %GATEWAY_PATH%

echo.
echo  ----------
echo  - UPLOAD -
echo  ----------
echo.

arduino-cli upload -p %GATEWAY_PORT% --fqbn arduino:avr:uno %GATEWAY_PATH%

echo.
echo ---------------------------------------------------------------------------------------
echo  SERIAL LISTENER
echo ---------------------------------------------------------------------------------------
echo.

start cmd /k arduino-cli monitor -p %NODE_PORT%
python SerialListener.py

echo.
echo ---------------------------------------------------------------------------------------
echo. 

endlocal
pause >nul