%% EECE 281 Lab 4
%  John Deppe, Theresa Mammarella, Steven Olsen

s1 = serial('COM4', 'BaudRate', 115200, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1, 'FlowControl', 'none');
fopen(s1);
val=fscanf(s1);
result = sscanf(val, '%f')
fclose(s1);