## --- CLOCK (100 MHz) ---
set_property PACKAGE_PIN Y9 [get_ports S_AXI_ACLK]
set_property IOSTANDARD LVCMOS33 [get_ports S_AXI_ACLK]
create_clock -period 10.000 -name sys_clk_pin -waveform {0.000 5.000} -add [get_ports S_AXI_ACLK]
## --- OLED INTERFACE (OLED_GPIO_BASE) ---
# SCLK
set_property PACKAGE_PIN AB12 [get_ports {oled_gpio_tri_io[0]}]
# SDIN
set_property PACKAGE_PIN AA12 [get_ports {oled_gpio_tri_io[1]}]
# RES
set_property PACKAGE_PIN U10 [get_ports {oled_gpio_tri_io[2]}]
# DC
set_property PACKAGE_PIN U9 [get_ports {oled_gpio_tri_io[3]}]
# VDD
set_property PACKAGE_PIN U11 [get_ports {oled_gpio_tri_io[4]}]
# VBAT
set_property PACKAGE_PIN U12 [get_ports {oled_gpio_tri_io[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports -filter { NAME =~ "oled_gpio*" }]
## --- SWITCHES (SW_GPIO_BASE) - Target Speed Input ---
set_property PACKAGE_PIN F22 [get_ports {sw_gpio_tri_io[0]}]
set_property PACKAGE_PIN G22 [get_ports {sw_gpio_tri_io[1]}]
set_property PACKAGE_PIN H22 [get_ports {sw_gpio_tri_io[2]}]
set_property PACKAGE_PIN F21 [get_ports {sw_gpio_tri_io[3]}]
set_property PACKAGE_PIN H19 [get_ports {sw_gpio_tri_io[4]}]
set_property PACKAGE_PIN H18 [get_ports {sw_gpio_tri_io[5]}]
set_property PACKAGE_PIN H17 [get_ports {sw_gpio_tri_io[6]}]
set_property PACKAGE_PIN M15 [get_ports {sw_gpio_tri_io[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports -filter { NAME =~ "sw_gpio*" }]
## --- PUSH BUTTONS (BTN_GPIO_BASE) - Controls ---
# BTN_RIGHT (Center/Manual Control)
set_property PACKAGE_PIN R18 [get_ports {btn_gpio_tri_io[0]}]
# BTN_LEFT
set_property PACKAGE_PIN N15 [get_ports {btn_gpio_tri_io[1]}]
# BTN_DOWN
set_property PACKAGE_PIN R16 [get_ports {btn_gpio_tri_io[2]}]
# BTN_UP
set_property PACKAGE_PIN T18 [get_ports {btn_gpio_tri_io[3]}]
# BTN_CENTER (Emergency Stop)
set_property PACKAGE_PIN P16 [get_ports {btn_gpio_tri_io[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports -filter { NAME =~ "btn_gpio*" }]## --- RESET ---
set_property PACKAGE_PIN P16 [get_ports S_AXI_ARESETN]
set_property IOSTANDARD LVCMOS33 [get_ports S_AXI_ARESETN]