clc, clear, close all;
clear divece
device = serialport("COM13",115200);

flag = 0;

period = 2;
point = 50;
length = (point*4)+4;
end_time = period*20e-3;
solve = end_time/(point-1);
time = 0:solve:end_time;

while(true)
    clc;
    while(flag == 0)
        one_bytes = uint8(read(device, 1, "uint8"));
        %disp(one_bytes)
        if(one_bytes == 143)
            %disp("Message found")
            end_of_reading = uint8(read(device, length-1, "uint8"));
            flag = 1;
        end
    end
    message = uint8(read(device, length, "uint8"));
    if(meta_verification(message) == 1)
        data = uint8(message(4:end-1));
        data_float = typecast(data, "single");
        disp("data read");
        plot_data(time, data_float);
    else
        flag = 0;
        close all;
    end
end

function plot_data(Xdata, Ydata)
    f1 = figure(1);
    plot(Xdata, Ydata, "LineWidth", 1, "Color", '#F54927');
    grid on
    title("График задаваемого напряжения")
    xlabel("Время, с")
    ylabel("Ток, А")
    ylim([-0.1 0.1]);%Current
    %ylim([-0.7 0.7]);%Power
    %ylim([9 15]);%Voltage
end

function status = meta_verification(message_checking)
    status = 0;
    header = uint8(message_checking(1:3));
    tail = uint8(message_checking(end));
    if(header(1) == 143)
        if(header(2) == 13)
            if(header(3) == 31)
                if(tail == 251)
                    status = 1;
                end
            end
        end
    end
end