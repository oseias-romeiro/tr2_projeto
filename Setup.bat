@echo off
@chcp 1252 >nul
color a
setlocal

@rem - Requisitos de instalação:
@rem arduino-cli lib install "LoRa"
@rem arduino-cli lib install "Ultrasonic"
@rem arduino-cli lib install "Sleep_n0m1"

@rem ---------------------------------------------------------------------------------------

echo. 
echo #----------------#
echo # Portas Serial: #
echo #----------------#
arduino-cli board list

@rem Solicita a porta NODE ao usuário
set /p NODE_PORT=Digite a porta do Node (exemplo: COM6): 

@rem Solicita a porta GATEWAY ao usuário
set /p GATEWAY_PORT=Digite a porta Gateway (exemplo: COM5): 

@rem caminho NODE
set NODE_PATH="%~dp0\Node\Node.ino"

@rem caminho GATEWAY
set GATEWAY_PATH="%~dp0\Gateway\Gateway.ino"

@rem ---------------------------------------------------------------------------------------
echo.

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

start cmd /k "echo -- Node -- & echo. & arduino-cli monitor -p %NODE_PORT%"
echo -- Gateway -- & echo.
python SerialListener.py %GATEWAY_PORT%

echo.
echo ---------------------------------------------------------------------------------------
echo. 

endlocal
pause >nul
