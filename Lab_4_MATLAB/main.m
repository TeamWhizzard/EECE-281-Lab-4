%% EECE 281 Lab 4
%  John Deppe, Theresa Mammarella, Steven Olsen

clear all
%close all
clc

s1 = serial('COM4', 'BaudRate', 115200, 'Parity', 'none', 'DataBits', 8, 'StopBits', 1, 'FlowControl', 'none');
fopen(s1);
N=input('Enter the number of readings to acquire: ');
M=input('Enter the number of readings to display: ');
result=zeros(3,M);
colors = ['r','g','b'];
titles = {'Temperature';'Humidity';'Light Level'};

for n=1:3
    axesHandles(n) = subplot(3,1,n);
    lineHandles(n) = plot(0,0,colors(n));
    title(titles(n))
    %set(axesHandles(n),'XTickLabel',fliplr(M:1),'XTickMode','Manual','XGrid','On','YGrid','On')
    set(lineHandles(n),'XData',[1:M])
end

for i=1:N
    val=fscanf(s1);
    result(:,i) = sscanf(val, '%f');
    
    for n=1:3
        subplot(3,1,n)
        if i<M
            slice = [NaN(1,M-i) result(n,1:i)];
        else
            range = [(i-M+1) : i];
            slice = result(n,range);
        end
        set(lineHandles(n),'YData',slice)
        %set(axesHandles(n),'XTick',labels)
    end
    drawnow
end
printf('Done!')
fclose(s1);