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

@rem porta
set PORT=COM4

@rem caminho Sender
set SENDER_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Sender\Sender.ino"

@caminho Receiver
set RECEIVER_PATH="D:\OneDrive - unb.br\[Matérias]\TR2\Projeto Final\tr2_projeto\Receiver\Receiver.ino"

@rem ---------------------------------------------------------------------------------------


@rem compile e upload do Sender
arduino-cli compile --fqbn arduino:avr:uno %SENDER_PATH%
arduino-cli upload -p %PORT% --fqbn arduino:avr:uno %SENDER_PATH%

@rem compile e faça o upload do Receiver
arduino-cli compile --fqbn arduino:avr:uno %RECEIVER_PATH%
arduino-cli upload -p %PORT% --fqbn arduino:avr:uno %RECEIVER_PATH%

endlocal
pause >nul