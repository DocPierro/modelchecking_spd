function compileMexFiles(debug)

if ~exist( 'debug', 'var' )
    debug = false;
end

% NDEBUG disables asserts and debugging output

if debug % DEBUG
    disp('DEBUG:');
    
    disp('ParetoFrontWFG');
    try
        mex -v -g CXXFLAGS="\$CXXFLAGS -Wall -std=gnu++98 -DBASE_2D" src/matlab/internal/ParetoFrontWFG.cpp -Isrc/matlab/contrib/eigen -outdir src/matlab/internal/
    catch ME
        disp(getReport(ME));
    end
    
    disp('spea2');
    try
        mex -v -g CXXFLAGS="\$CXXFLAGS -Wall -std=gnu++98" src/matlab/contrib/+spea2/source/spea2.cpp src/matlab/contrib/+spea2/source/variator.cpp src/matlab/contrib/+spea2/source/selector.cpp -outdir src/matlab/contrib/+spea2/
    catch ME
        disp(getReport(ME));
    end
    
    disp('SMS-EMOA');
    try
        mex -v -g CXXFLAGS="\$CXXFLAGS -Wall" src/matlab/contrib/sms-emoa/hv.cpp src/matlab/contrib/sms-emoa/Hypervolume.cpp -Isrc/matlab/contrib/sms-emoa/ -outdir src/matlab/contrib/sms-emoa/
        mex -v -g CFLAGS="\$CFLAGS -O2 -Wall" src/matlab/contrib/sms-emoa/paretofront.c -outdir src/matlab/contrib/sms-emoa/
    catch ME
        disp(getReport(ME));
    end
    
else % NDEBUG - RELEASE
    disp('RELEASE');

    disp('ParetoFrontWFG');
    try
        mex CXXFLAGS="\$CXXFLAGS -O2 -std=gnu++98 -march=native -DNDEBUG -DBASE_2D" src/matlab/internal/ParetoFrontWFG.cpp -Isrc/matlab/contrib/eigen -outdir src/matlab/internal/
    catch ME
        disp(getReport(ME));
    end
    
    disp('spea2');
    try
        mex CXXFLAGS="\$CXXFLAGS -O2 -std=gnu++98 -march=native -DNDEBUG" src/matlab/contrib/+spea2/source/spea2.cpp src/matlab/contrib/+spea2/source/variator.cpp src/matlab/contrib/+spea2/source/selector.cpp -outdir src/matlab/contrib/+spea2/
    catch ME
        disp(getReport(ME));
    end

    disp('SMS-EMOA');
    try
        mex CXXFLAGS="\$CXXFLAGS -O2 -march=native -DNDEBUG" src/matlab/contrib/sms-emoa/hv.cpp src/matlab/contrib/sms-emoa/Hypervolume.cpp -Isrc/matlab/contrib/sms-emoa/ -outdir src/matlab/contrib/sms-emoa/
        mex CFLAGS="\$CFLAGS -O2 -march=native -DNDEBUG" src/matlab/contrib/sms-emoa/paretofront.c -outdir src/matlab/contrib/sms-emoa/
    catch ME
        disp(getReport(ME));
    end
end