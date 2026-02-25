clc, clear, close all;
clear divece
divece = serialport("COM9",115200);

period = 2;
point = 50;
end_time = period*20e-3;
solve = end_time/(point-1);

time = 0:solve:end_time;

while(true)
   message = uint8(read(divece, (point*4)+4, "uint8"));
   disp(message)
   header = uint8(message(1:3));
   tail = uint8(message(end));
   if(header(1) == 143)
      if(header(2) == 13)
          if(header(3) == 31)
              if(tail == 251)
                data = uint8(message(4:end-1));
                data_float = typecast(data, "single");
                plot(time, data_float, "LineWidth", 1, "Color", '#F54927');
                grid on
                title("График задаваемого напряжения")
                xlabel("Время, с")
                ylabel("Напряжение, В")
                ylim([-260 260]);
              end
          end
       end
    end   
end

% x = 0;
% while(x == 0)
%     message = uint8(read(divece, 1, "uint8"));
%     disp(message)
%     if(message == 141)
%         disp("Nice")
%         x = 1;
%     end
% end