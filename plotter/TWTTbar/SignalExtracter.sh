#echo "Starts an interactive sesion of 20 cores"
#qsub -I -l nodes=1:ppn=20

echo "First of all, running plotvariables.py"
python plotvariables.py 20

echo "Then we start to obtain the signal from the minitrees..."

echo "(one / three) running getFinalCards.py for all the variables selected in varList"
python getFinalCards.py 'All' 20

echo "(two / three) running getClosureHistos.py for all the variables selected in varList"
python getClosureHistos.py 'LeadingLepPt'
python getClosureHistos.py 'LeadingJetPt'
python getClosureHistos.py 'M_bl_minmax_ATLAS'
python getClosureHistos.py 'M_bl_minmax_opt'

echo "(three / three) running FinalExtracter.py for all the variables selected in varList"
python FinalExtracter.py 'LeadingLepPt'
python FinalExtracter.py 'LeadingJetPt'
python FinalExtracter.py 'M_bl_minmax_ATLAS'
python FinalExtracter.py 'M_bl_minmax_opt'


echo "After that, getting the response matrices..."
python getMatrices.py

echo "And finally, unfolding..."
python unfoldTWTTbar.py 'LeadingLepPt'
python unfoldTWTTbar.py 'LeadingJetPt'
python unfoldTWTTbar.py 'M_bl_minmax_ATLAS'
python unfoldTWTTbar.py 'M_bl_minmax_opt'


echo "Coping all the plots to the personal website www.hep.uniovi.es/ireneaj/"
source copier.sh p
