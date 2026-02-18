clc, clear, close all;
divece = serialport("COM9",9600);

num = 3+1;

while(true)
   data = read(divece, num, "single");
   disp(data);
end