onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb/rst
add wave -noupdate /tb/ena
add wave -noupdate /tb/clk
add wave -noupdate /tb/DetectionCode
add wave -noupdate -radix decimal /tb/x12
add wave -noupdate /tb/detector1
add wave -noupdate /tb/detector2
add wave -noupdate -radix decimal /tb/x3
add wave -noupdate /tb/detector3
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {39722136 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {13004800 ps} {15052800 ps}
