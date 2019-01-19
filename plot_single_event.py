#!/usr/bin/python

import ROOT
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(000000000)
ROOT.gStyle.SetOptTitle(0)

from ROOT import TCanvas, TFile, THStack, TLegend, gDirectory, gStyle, TH3D


import time 

def plot(filename,name='plots/Pion'):
    #load root file 
    r_file = TFile(filename)
    r_tree = r_file.Get('demo/FitParametersTree')
    #gStyle->SetPalette(55);
    #gStyle.SetPalette(51)
    c1 = TCanvas( 'c1', '20 GeV particles in the calorimeter' )
    for i in xrange(2,3):
        print 'number of histograms',i
        myhist = TH3D('hist_'+str(i),'Particle number '+str(i),100,-27.5,27.5,100,-27.5,27.5,100,0,50)
        #r_tree.Draw('(myz-myz[0])/10:(myy-myy[0])/10:(myx-myx[0])/10:myenergy/Sum$(myenergy)>>hist_'+str(i)+'(100,-25,25,100,-25,25,100,0,50)', 'abs(myz-myz[0])/10<30 && abs(myy-myy[0])/10<30 && abs(myx-myx[0])/10<110  && Entry$=='+str(i),'')
        r_tree.Draw('(myz-myz[0])/10:(myy-myy[0])/10:(myx-myx[0])/10:myenergy>>+hist_'+str(i), 'abs(myz-myz[0])/10<35 && abs(myy-myy[0])/10<35 && abs(myx-myx[0])/10<110  && Entry$=='+str(i),'')

        hist = gDirectory.Get("hist_"+str(i))
        hist.GetXaxis().SetNdivisions(409)
        hist.GetYaxis().SetNdivisions(409)
        hist.GetZaxis().SetNdivisions(409)
        hist.GetXaxis().SetTitle('x [cm]')
        hist.GetYaxis().SetTitle('y [cm]')
        hist.GetZaxis().SetTitle('z [cm]')
        hist.GetXaxis().SetTitleOffset(1.5)
        hist.GetYaxis().SetTitleOffset(1.7)
        hist.GetZaxis().SetTitleOffset(1.1)
        c1.Update()
        c1.SaveAs(name+'_'+str(i)+'.pdf')
    

if __name__ == '__main__':
    #plot('fullsim/example_pi20Gev.root')
    plot('fullsim/example_ele_20GeV.root','plots/Ele')

