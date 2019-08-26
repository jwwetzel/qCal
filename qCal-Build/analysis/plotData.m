
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Electrons GeV 
dataGeV = csvread('el_GeVif.csv'); 
energiesGeV = dataGeV(2,2:end); % energies are row 2
meansGeV = dataGeV(3,2:end); % means are row 3
stdevsGeV = dataGeV(4,2:end); % stdevs are row4 
sdiveGeV = zeros(1, length(energiesGeV)); % this is stdev / energy
sdivmGeV = zeros(1, length(energiesGeV)); % this is stdev / means which is the energy resolution

for i = 1:length(energiesGeV)
    sdiveGeV(i) = stdevsGeV(i)/energiesGeV(i);
    sdivmGeV(i) = stdevsGeV(i)/meansGeV(i);
    disp(stdevsGeV(i));
    disp(energiesGeV(i));
    disp(stdevsGeV(i)/energiesGeV(i));
end

fsize = 14;
% Plotting Means for GeV
figure(4); % BOGUS FIT CURVE 

myfit = fittype('a + b*x',...
'dependent',{'y'},'independent',{'x'},...
'coefficients',{'a','b'});
fitcurve1 = fit(energiesGeV', meansGeV', myfit);
plot(fitcurve1,'k', energiesGeV, meansGeV, 'rs');

ylabel("Peak position of the number of hits", 'FontSize', fsize);
xlabel("Beam Energy [GeV]",'horizontalAlignment', 'left','FontSize', fsize);
title("");
legend off;
savefig('means.fig');
% Plotting Stds for GeV
figure(5); % GOOD

myfit = fittype('a*sqrt(x) + b*x',...      %'x*sqrt(abs(a/x.^2 + b/x + c))'
'dependent',{'y'},'independent',{'x'},...
'coefficients',{'a','b'}); % , 'Lower', [0,0,0]
fitcurve2 = fit(energiesGeV', stdevsGeV', myfit);
plot(fitcurve2,'k', energiesGeV, stdevsGeV, 'rs');
xlabel("Beam Energy E [GeV]",'horizontalAlignment', 'left', 'FontSize', fsize);
ylabel("\sigma(E)", 'FontSize', fsize);
title("");
legend off;
%text(300, 1e+05
savefig('stdevs.fig');
% Plotting stds/E for GeV
figure(6); % GOOD
 
myfit = fittype('a/sqrt(x) + b',... % 'sqrt(abs(a/x.^2 + b/x + c))'
'dependent',{'y'},'independent',{'x'},...
'coefficients',{'a','b'});
fitcurve3 = fit(energiesGeV', sdivmGeV', myfit);
plot(fitcurve3,'k', energiesGeV, sdivmGeV, 'rs');
xlabel("Beam Energy E [GeV]", 'horizontalAlignment', 'left', 'FontSize', fsize);
ylabel("\sigma(E)/E " , 'FontSize', fsize);
title("");
legend off;
bval = 100*fitcurve3.b;
aval = 100*fitcurve3.a;
mytxt = [num2str(100*fitcurve3.b), "% + ",  num2str(100*fitcurve3.a), " %/\surd(E)"]; 
xoff = 100;
yoff = 0.02;
text(100 + xoff, 0.1 + yoff, mytxt(1));
text(180 + xoff, 0.1 + yoff, mytxt(2));
text(220 + xoff, 0.1 + yoff, mytxt(3));
text(280 + xoff, 0.1 + yoff, mytxt(4));
savefig('energyRes.fig');


%{
Formatting Info:
xlabel should be right justified 
no legend unless multiple plots 
data should be red, easy to see
fit curves should be black
fit parameters should be shown 

%} 
% for means there is a linear region and a logarithmic region
% no use trying to fit a single curve over the whole thing 
% break starts around 25 MeV