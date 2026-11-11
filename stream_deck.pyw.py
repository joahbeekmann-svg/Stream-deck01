import serial
import pyautogui
import time
import obsws_python as obs

COM_PORT = 'COM7'  # Passe deinen COM-Port an
BAUD_RATE = 115200

# Verbindung zu OBS herstellen (Port 4455 ist Standard)
obs_client = None
try:
    obs_client = obs.ReqClient(host='localhost', port=4455, password='')
    print("Mit OBS verbunden!")
except Exception as e:
    print(f"Konnte nicht mit OBS verbinden: {e}")

print(f"Verbinde mit ESP32 an {COM_PORT}...")

try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print("Verbunden mit ESP32!")

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            
            if line.startswith("BTN:"):
                button = line.split(":")[1]
                print(f"Knopf empfangen: {button}")

                # 1. Gelb: Taskmanager
                if button == "GELB":
                    pyautogui.hotkey('ctrl', 'shift', 'esc')

                # 2. Rot: Mute ('m')
                elif button == "ROT":
                    pyautogui.press('m')

                # 3. Grün: YouTube Play/Pause & Leertaste
                elif button == "GRUEN":
                    pyautogui.press('playpause')
                    pyautogui.press('space')

                # 4. Schwarz: OBS Szene 2 direkt umschalten
                elif button == "SCHWARZ":
                    if obs_client:
                        try:
                            obs_client.set_current_program_scene("ende") # HIER DEINEN OBS-SZENENNAMEN EINTRAGEN
                        except Exception as err:
                            print(f"OBS Fehler: {err}")

                # 5. Blau: OBS Szene 1 direkt umschalten
                elif button == "BLAU":
                    if obs_client:
                        try:
                            obs_client.set_current_program_scene("minecraft") # HIER DEINEN OBS-SZENENNAMEN EINTRAGEN
                        except Exception as err:
                            print(f"OBS Fehler: {err}")

        time.sleep(0.01)

except serial.SerialException:
    print(f"FEHLER: Konnte keine Verbindung zu {COM_PORT} herstellen.")
except KeyboardInterrupt:
    print("\nSkript beendet.")
