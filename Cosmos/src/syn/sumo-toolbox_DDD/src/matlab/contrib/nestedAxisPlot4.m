function nestedAxisPlot4(model, outputIndex, Div, Cont)
%
% ======================================================================================================
% This function is based on the function nested4.m from http://www.wiley.com//legacy/wileychi/forrester/
% The code is licensed under LGPL v3 or later http://www.gnu.org/licenses/lgpl.html
% The code was modified to accept a SUMO model object
% ======================================================================================================
%
% Generates a four variable nested axis plot
%
% Inputs:
%		Varorder - four-element vector specifying the assignment of the
%			      variables to each of the four axes [main horizontal 
%		                mani vertical, tile horizontal, tile vertical ]
%		Div - 1x2 vector of two variables specifying the number of tiles along
%		        the main horizontal and main vertical axis respectively
%		Range - 2x4 matrix of minimum and maximum values for each
%                            variable
%		Labels - cell array containing the names of the variables
% 		Objhandle - name of the objective function
%		Mesh - the objective function will be sampled on a mesh x mesh full
%              		factorial plan on each tile
% 		Lower/Upper- minimum/maximum value of the objective function - this is 
%              			required to adjust the colour range of each tile with
%              			respect to the full colour range of the function (if not
%              			known, set to [] and the function will estimate it).
% 		Cont- if Cont = 1 contour lines are plotted and the spaces between
%              	        them are filled with a colour determined by the function
%              	        value. Otherwise a colour-shaded plot is generated.

[in out] = model.getDimensions();

if(in ~= 4)
  error('Sorry, this function can only be used with models with 4 inputs')
end

% Get properties from SUMO object

Varorder = 1:4;

% figure out the original range
pm = [-1 -1 -1 -1 ; 1 1 1 1];
[inFunc outFunc] = model.getTransformationFunctions();
ps = outFunc(pm);
Range = ps;

Labels = model.getInputNames();
Objhandle = @(x)model.evaluate(x);
Mesh = 13;
Lower = [];
Upper = [];

if(~exist('outputIndex','var'))
  outputIndex = 1;
end

if(~exist('Cont','var'))
  Cont = 1;
end

if(~exist('Div','var'))
  Div = [7 7];
end

% Greying out the end of the colormap - this will be one of the colours
% used to represent infeasible regions. Only available for colour-shaded
% plots (Cont~=1).

if Cont~=1
    Map = colormap;
    Map(end,1:3) = [.5 .5 .5];
    colormap(Map);
end

MainX = Varorder(1);
MainY = Varorder(2);
SubX  = Varorder(3);
SubY  = Varorder(4);

MainX_divisions = Div(1);
MainY_divisions = Div(2);

tile_width = 0.95/(MainX_divisions+1);
tile_height = 0.95/(MainY_divisions+1);

% The inter-tile gap as a fraction of the width of the full plot
gap = 0.005;

% Data labels font size
Fs = 16;


% Data limits for colour axis control
lo = inf;
hi = -inf;

figure

for i=1:MainY_divisions
    for j=1:MainX_divisions
        
        % Tile in row i, column j, as counted from top left
        % For MainX and MainY we take the value at the origin of the tile
        
        MainX_value = Range(1,MainX) +...
            (j-1)*(Range(2,MainX)-Range(1,MainX))/...
            MainX_divisions;
        
        MainY_value = Range(1,MainY) +...
            (MainY_divisions-i)*(Range(2,MainY)-Range(1,MainY))/...
            MainY_divisions;
        
        
        SubXrange = Range(1,SubX):...
            (Range(2,SubX)-Range(1,SubX))/(Mesh-1):...
                    Range(2,SubX);
                
        SubYrange = Range(1,SubY):...
            (Range(2,SubY)-Range(1,SubY))/(Mesh-1):...
                    Range(2,SubY);
        
        [X,Y] = meshgrid(SubXrange,SubYrange);
        
        
        M = zeros(size(X));
        % Computing tile i,j
        for l = 1:size(X,1)
            for ll = 1:size(X,2)
                design = zeros(1,4);
                design(MainX) = MainX_value;
                design(MainY) = MainY_value;
                design(SubX) = X(l,ll);
                design(SubY) = Y(l,ll);
                tmp = feval(Objhandle,design);
		M(l,ll) = tmp(:,outputIndex);
                if M(l,ll)>hi, hi=M(l,ll); end
                if M(l,ll)<lo, lo=M(l,ll); end                
            end
        end
        
        % Plotting the current tile
        axes;
        hold on
        set(gca,'Position',[tile_width/2+(j-1)*tile_width,...
            tile_height/2+(MainY_divisions-i)*tile_height,...
            tile_width-gap,tile_height-gap])
        
        
        if Cont == 1
            contourf(X,Y,M)
            % If you don't want the contourlines to show, use:
            % contourf(X,Y,M,'LineStyle','none')
            % instead of the line above
        else
            pcolor(X,Y,M), shading interp, axis tight
        end
        
     
        
        set(gca,'XTickLabel',[])
        set(gca,'YTickLabel',[])
        set(gca,'FontSize',12)
        
        
        
        if ~isempty(Lower) && ~isempty(Upper)
            caxis([Lower Upper])
        else
            caxis([lo hi]);
            Upper = hi;
            Lower = lo;
        end

        if j==1
            if i==MainY_divisions
                ylabel(strcat('\bf{',Labels(MainY),'=}',...
                    num2str(MainY_value,2)),'FontSize',Fs,'Interpreter','tex');
            else
                ylabel(num2str(MainY_value,2),'FontSize',Fs,'Interpreter','tex');
            end
        end
        if i==MainY_divisions
            if j==1
                xlabel(strcat('\bf{',Labels(MainX),'=}',...
                    num2str(MainX_value,2)),'FontSize',Fs,'Interpreter','tex');
            else
                xlabel(num2str(MainX_value,2),'FontSize',Fs,'Interpreter','tex');
            end                
        end
    end
end

colorbar('OuterPosition',[0.91 -0.028 0.06 1.015])


fprintf('The extrema of the evaluated objective function values are:\n');
fprintf('Minimum: %6.5f\n',lo);
fprintf('Maximum: %6.5f\n',hi);

% If the user-specified objective function value range (which is needed to
% specify the colour range for each tile) is outside the actual objective
% function range by more than 5% either side...
if abs(Lower-lo)>abs(0.05*lo) || abs(Upper-hi)>abs(0.05*hi)
    fprintf('You may wish to re-run the function with these values\n');
    fprintf('as input parameters Lower and Upper.\n');
end
    