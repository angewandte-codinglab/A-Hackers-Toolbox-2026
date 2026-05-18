# Troubleshooting

### 1. COM port blocked by another process
Another program (like a serial monitor left open) may be holding the port. Fix:

    Close any open serial monitors (Arduino IDE, PuTTY, etc.) before uploading
Check Task Manager for lingering processes


### 2. ESP32-S3 stuck / not in bootloader mode
The ESP32-S3 sometimes doesn't auto-reset into download mode on Windows. 

### 3. Run IDE as Administrator
If the port is there but access is denied, the IDE may lack permissions:

    Right-click Arduino IDE  → Run as administrator


### 4. Fix COM port permissions via Device Manager

    Open Device Manager
    Find the ESP32 under Ports (COM & LPT)
    Right-click → Properties → Port Settings → Advanced
    
    Try changing the COM port number (high numbers like COM10+ can cause issues — reassign to COM3 or COM4)
