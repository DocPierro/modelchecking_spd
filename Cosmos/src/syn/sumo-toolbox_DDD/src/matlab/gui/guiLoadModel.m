% > @file "guiLoadModel.m"
% > @authors SUMO Lab Team
% > @version 2014a
% > @date Copyright 2006-2014
% >
% > This file is part of the Surrogate Modeling Toolbox ("SUMO Toolbox")
% > and you can redistribute it and/or modify it under the terms of the
% > GNU Affero General Public License version 3 as published by the
% > Free Software Foundation.  With the additional provision that a commercial
% > license must be purchased if the SUMO Toolbox is used, modified, or extended
% > in a commercial setting. For details see the included LICENSE.txt file.
% > When referring to the SUMO Toolbox please make reference to the corresponding
% > publication:
% >   - A Surrogate Modeling and Adaptive Sampling Toolbox for Computer Based Design
% >   D. Gorissen, K. Crombecq, I. Couckuyt, T. Dhaene, P. Demeester,
% >   Journal of Machine Learning Research,
% >   Vol. 11, pp. 2051-2055, July 2010.
% >
% > Contact : sumo@sumo.intec.ugent.be - http://sumo.intec.ugent.be
% > Signature
% >	[model filename] = guiLoadModel(defaultFilename)
% 
%  ======================================================================
% > @brief Load a model from file, by showing the user a dialog for selecting
% >the file to open.
% >
% > - The dialog will open in the directory specified, if it exists.
% > - If the specified path also contains a filename, that name will be
% > entered in the name field.
% > - If the specified path is invalid, the current directory will be shown.
% > - If the load fails (e.g. user closes the dialog), the return
% > parameters will be empty.
% >
% > @param defaultFilename default name of the file to load
% > @retval model the loaded model, or [] if load failed
% > @retval filename	the file path of the loaded file containing the model
% >
% > Examples:
% > - model = guiLoadModel('/home/')
% > - [model filename] = guiLoadModel('D:\\Workspace')
%  ======================================================================
%
