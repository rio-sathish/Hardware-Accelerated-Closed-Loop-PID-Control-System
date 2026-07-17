Export the Hardware Design to Vitis
Once the bitstream is generated, export the design to Vitis where the software interface program is
written. This program sends mode commands to the custom LED IP and controls the hardware
behavior.
1. First, make sure the ZedBoard is powered on and connected via JTAG.
2. In Vivado, go to the Flow Navigator and click Open Hardware Manager → Open Target → Open
New Target. The New Hardware Target wizard appears.
3. Click Next, select the hardware target (the ZedBoard should appear), click Next, then Finish.
4. Click Refresh Device, then click Program Device. A dialog appears.
5. Select the generated bitstream file and click Program. The FPGA is now programmed with the
hardware design.
6. From the Vivado file menu, select File → Export → Export Hardware.7. In the window that appears, select "Fixed" and click "Next".
8. Select "Include Bitstream" and click "Next".
9. Browse to the desired folder, give the file a name, and click "Next" then "Finish". A .xsa file is
created containing the hardware description and bitstream.
10. From the Tools menu, select "Launch Vitis IDE".
11. In the workspace selection window, browse to a workspace directory and click "Launch".6. Create the Software Application
1. In Vitis, click File → New → Application Project (or use Create Application Component in Vitis
2025.1).
2. In the window that appears, click "Next".
3. Click "Create a new platform from hardware (XSA)", browse to the .xsa file exported from
Vivado, and click "Next".
4. Type the Application Project Name as accel_app, select the MicroBlaze processor, and click
"Next".5. In the template window, select "microblaze_code" and click "Finish".
6. The project files are added to the Project Explorer window, including the main C source file.
7. Double-click the main .c file and replace its contents with the LED accelerator program.
8. After editing the program, right-click the project folder and select "Build Project" to compile.9. Afterthe project is built,right-click the projectfolderandselectRun As → Launch on Hardware.
Vitis automatically programs the FPGA and loads the executable into the MicroBlaze BRAM.
10. The output appears on JTAG terminal. The main menu is displayed and the system waits for a key
input.
11. The OLED BEHAVIOUR on the board changes according to the PUSH BUTTONselected.
Thus the command has been successfully sent from the PC to the MicroBlaze processor, which writes
to the custom IP registers, and the hardware responds by driving the LEDs accordingly.Software Application Program