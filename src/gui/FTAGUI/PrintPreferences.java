/******************************************************************************
** 
** OpenFTA - Fault Tree Analysis
** Copyright (C) 2005 FSC Limited
** 
** This program is free software; you can redistribute it and*or modify it
** under the terms of the GNU General Public License as published by the Free
** Software Foundation; either version 2 of the License, or (at your 
** option) any later version.
** 
** This program is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
** more details.
**
** You should have received a copy of the GNU General Public License along 
** with this program; if not, write to the Free Software Foundation, Inc., 
** 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
**
** To contact FSC, please send an email to support@fsc.co.uk or write to 
** FSC Ltd., Cardiff Business Technology Centre, Senghenydd Road, Cardiff,
** CF24 4AY.
**
******************************************************************************/

package FTAGUI;

import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import javax.swing.border.*;
import javax.accessibility.*;

import java.awt.*;
import java.awt.event.*;
import java.beans.*;
import java.util.*;
import java.io.*;
import java.applet.*;
import java.net.*;
import java.lang.*;
import java.awt.print.*;

public class PrintPreferences extends JDialog {

    private PrintPreviewCanvas canvas;
    public static int numPagesX = 1;
    public static int numPagesY = 1;
    private int zoom = 30;

    public static PrinterJob printerJob = PrinterJob.getPrinterJob();
    public static PageFormat pageFormat = printerJob.defaultPage();

    private int localNumPagesX = 1;
    private int localNumPagesY = 1;
    private int localZoom = 30;

    private int pageSeperation = 10;
    
    private JTextField pagesX;
    private JTextField pagesY;
    private JTextField zoomText;
    private JButton upButton;
    private JButton downButton;
    private JLabel numPagesXLabel;
    private JLabel numPagesYLabel;
    private JLabel zoomLabel;
    private ImageIcon upArrowIcon;
    private ImageIcon downArrowIcon;

    public PrintPreferences(Frame parent) {

	super(parent);

	// setup a simple frame containing only an FTACanvas
	getContentPane().setLayout(new BorderLayout());

	setSize(550, 400);
	setTitle("Print Preview");
	setModal(true);

	JScrollPane canvasScrollPane = new JScrollPane();
	canvasScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
	canvasScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	canvas = new PrintPreviewCanvas(localNumPagesX, localNumPagesY, zoom, pageSeperation, canvasScrollPane.getViewport());
	canvasScrollPane.setViewportView(canvas);

	getContentPane().add(canvasScrollPane, BorderLayout.CENTER);

	upArrowIcon = FTAUtilities.loadImageIcon("upArrow.gif");
	downArrowIcon = FTAUtilities.loadImageIcon("downArrow.gif");

	numPagesXLabel = new JLabel();

	Box numPagesXArrowBox = new Box(BoxLayout.Y_AXIS);
      pagesX = new JTextField(new RestrictedDocument("0123456789"), "1", 3);
      pagesX.addActionListener(new ActionListener() {
             public void actionPerformed(ActionEvent e) {
                try {
                	localNumPagesX = Integer.parseInt(pagesX.getText());
                } catch(Exception x) {
                  //number format exception should not be thrown as 
                  // document text is restricted.
                }
                canvas.setNumPagesX(localNumPagesX);
		    //updateLabels();
		    updateDialog();
            }
          });  
                
	JButton numPagesXUpButton = new JButton(upArrowIcon);
	numPagesXUpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    localNumPagesX++;
		    canvas.setNumPagesX(localNumPagesX);
		    //updateLabels();
                pagesX.setText(""+localNumPagesX);
		    updateDialog();
		}
	    });

	JButton numPagesXDownButton = new JButton(downArrowIcon);
	numPagesXDownButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(localNumPagesX > 1) {
			localNumPagesX--;
			canvas.setNumPagesX(localNumPagesX);
			//updateLabels();
                  pagesX.setText(""+localNumPagesX);
			updateDialog();
		    }
		}
	    });

	numPagesXArrowBox.add(numPagesXUpButton);
	numPagesXArrowBox.add(numPagesXDownButton);

	numPagesYLabel = new JLabel();
      pagesY = new JTextField(new RestrictedDocument("0123456789"), "1", 3);
      pagesY.addActionListener(new ActionListener() {
             public void actionPerformed(ActionEvent e) {
                try {
                	localNumPagesY = Integer.parseInt(pagesY.getText());
                } catch(Exception x) {
                  //number format exception should not be thrown as 
                  // document text is restricted.
                }
                canvas.setNumPagesY(localNumPagesY);
		    //updateLabels();
		    updateDialog();
            }
          });  

	Box numPagesYArrowBox = new Box(BoxLayout.Y_AXIS);

	JButton numPagesYUpButton = new JButton(upArrowIcon);
	numPagesYUpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    localNumPagesY++;
		    canvas.setNumPagesY(localNumPagesY);
		    //updateLabels();
                pagesY.setText(""+localNumPagesY);
		    updateDialog();
		}
	    });

	JButton numPagesYDownButton = new JButton(downArrowIcon);
	numPagesYDownButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(localNumPagesY > 1) {
			localNumPagesY--;
			canvas.setNumPagesY(localNumPagesY);
			//updateLabels();
                  pagesY.setText(""+localNumPagesY);
			updateDialog();
		    }
		}
	    });

	numPagesYArrowBox.add(numPagesYUpButton);
	numPagesYArrowBox.add(numPagesYDownButton);

	zoomLabel = new JLabel();
      zoomText = new JTextField(new RestrictedDocument("0123456789"), "1", 3);
      zoomText.addActionListener(new ActionListener() {
             public void actionPerformed(ActionEvent e) {
                try {
                	zoom = Integer.parseInt(zoomText.getText());
                } catch(Exception x) {
                  //number format exception should not be thrown as 
                  // document text is restricted.
                }
                canvas.setZoom(zoom);
		    //updateLabels();
		    updateDialog();
            }  
          });

	Box zoomArrowBox = new Box(BoxLayout.Y_AXIS);

	JButton zoomUpButton = new JButton(upArrowIcon);
	zoomUpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    zoom += 10;
		    //updateLabels();
                zoomText.setText(""+zoom);
		    canvas.setZoom(zoom);
		    updateDialog();
		}
	    });

	JButton zoomDownButton = new JButton(downArrowIcon);
	zoomDownButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    if(zoom > 0.1f) {
			zoom -= 10;
			//updateLabels();
                  zoomText.setText(""+zoom);
			canvas.setZoom(zoom);
			updateDialog();
		    }
		}
	    });

	zoomArrowBox.add(zoomUpButton);
	zoomArrowBox.add(zoomDownButton);


	JPanel sizePanel = new JPanel(new FlowLayout());

	sizePanel.add(numPagesXLabel);
      sizePanel.add(pagesX);
	sizePanel.add(numPagesXArrowBox);
	sizePanel.add(numPagesYLabel);
      sizePanel.add(pagesY);
	sizePanel.add(numPagesYArrowBox);
	sizePanel.add(zoomLabel);
      sizePanel.add(zoomText);
	sizePanel.add(zoomArrowBox);
    
/*
	JButton printerSetupButton = new JButton("Printer Setup...");
	printerSetupButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    showPrinterSetupDialog();
		}
	    });

	JButton pageSetupButton = new JButton("Page Setup...");
	pageSetupButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    showPageSetupDialog();
		    updateDialog();
		}
	    });
      JButton printButton = new JButton ("Print...");
      printButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent e) {
             FTACanvas ftaCanvas = canvas.getFTACanvas();
             float scale = ftaCanvas.getScale();
	       // set scale to 1.0 so print will fill page
	       ftaCanvas.setScale(1.0f);
	       PrintUtilities.printComponent(ftaCanvas, localNumPagesX, 
                localNumPagesY, printerJob, pageFormat);
	      // restore correct scale
	      ftaCanvas.setScale(scale);
        }
    });

*/

        JToolBar toolBar = new JToolBar();
	toolBar.setFloatable(false);


	JButton printSetupButton = FTAUtilities.createButton("print setup");
	toolBar.add(printSetupButton);
	JButton pageSetupButton = FTAUtilities.createButton("page setup");
	toolBar.add(pageSetupButton);
	JButton printButton = FTAUtilities.createButton("print tree");
	toolBar.add(printButton);

	printSetupButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
                    showPrinterSetupDialog();
		    //updateDialog();
		}
	    });

	pageSetupButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
                    showPageSetupDialog();
		    //updateDialog();
		}
	    });


	printButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
                    FTACanvas ftaCanvas = canvas.getFTACanvas();
                    float scale = ftaCanvas.getScale();
	            // set scale to 1.0 so print will fill page
	            ftaCanvas.setScale(1.0f);
	            PrintUtilities.printComponent(ftaCanvas, localNumPagesX, 
                    localNumPagesY, printerJob, pageFormat);
	            // restore correct scale
	            ftaCanvas.setScale(scale);
		}
	    });

	JButton okButton = new JButton("OK");
	okButton.setDefaultCapable(true);
	getRootPane().setDefaultButton(okButton);
	okButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    numPagesX = localNumPagesX;
		    numPagesY = localNumPagesY;
		    zoom = localZoom;
		    setVisible(false);
		}
	    });

	JButton cancelButton = new JButton("Cancel");
	cancelButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    setVisible(false);
		}
	    });

	JButton helpButton = new JButton("Help");
	helpButton.addActionListener(new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    BrowserLauncher.openHelpFile(new String("fta_dialog_print_preview.html"));
		}
	    });
/*
	JPanel topButtonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
	topButtonPanel.add(printerSetupButton);
	topButtonPanel.add(pageSetupButton);
      topButtonPanel.add(printButton);
*/
	JPanel bottomButtonPanel = new JPanel(new FlowLayout());
	bottomButtonPanel.add(okButton);
	bottomButtonPanel.add(cancelButton);
	bottomButtonPanel.add(helpButton);
       
	JPanel bottomPanel = new JPanel(new GridLayout(2,1));

	bottomPanel.add(sizePanel);
	bottomPanel.add(bottomButtonPanel);

	getContentPane().add(bottomPanel, BorderLayout.SOUTH);
//	getContentPane().add(topButtonPanel, BorderLayout.NORTH);
        getContentPane().add(toolBar, BorderLayout.NORTH);
	updateLabels();

    }

    public void showPrinterSetupDialog() {
	//printerJob.printDialog();
	//pageFormat = printerJob.validatePage(pageFormat);

	Thread showPrinterSetupThread = new Thread(new RunPrinterSetupDialog(this));
	showPrinterSetupThread.start();
    }

    public void showPageSetupDialog() {
	//pageFormat = printerJob.pageDialog(pageFormat);

	Thread showPageSetupThread = new Thread(new RunPageSetupDialog(this));
	showPageSetupThread.start();
    }


    public void showDialog() {
	localNumPagesX = numPagesX;
	localNumPagesY = numPagesY;
	localZoom = zoom;
	canvas.setNumPagesX(localNumPagesX);
	canvas.setNumPagesY(localNumPagesY);
	canvas.setZoom(localZoom);
	
	updateLabels();
	updateDialog();

	setVisible(true);
    }
   
    private void updateLabels() {
      numPagesXLabel.setText("Horizontal Pages");
      numPagesYLabel.setText("Vertical Pages");
      zoomLabel.setText ("Zoom is ");
      pagesX.setText(""+localNumPagesX);
      pagesY.setText(""+localNumPagesY);
      zoomText.setText(""+zoom);
      /*
	if(localNumPagesX == 1) {
	    numPagesXLabel.setText("1 Horizontal Page");
	} else {
	    numPagesXLabel.setText(localNumPagesX + " Horizontal Pages");
	}
	if(localNumPagesY == 1) {
	    numPagesYLabel.setText("1 Vertical Page");
	} else {
	    numPagesYLabel.setText(localNumPagesY + " Vertical Pages");
	}
      
	zoomLabel.setText("Zoom Is " + zoom + "%");*/
    }


    public void updateDialog() {
        // no need to update if dialog is not on screen
        if(isShowing()) {
            
            // ensure that all painting is done on Swing thread
            if(!SwingUtilities.isEventDispatchThread()) {
                try {
                    SwingUtilities.invokeAndWait( new Runnable(){
                            public void run(){
                                // ensure that mouse events are handled on the swing thread
                                // as advanced zoom draws directly on to the canvas
                                updateDialog();
                            }
                        });
                }
                catch (Exception e) {
                    e.printStackTrace();
                }   
            } else {
                canvas.updatePrintPreviewImage();
                canvas.invalidate();
                validate();
                repaint();
                canvas.repaint();
            }
        }
    }


}


class RunPageSetupDialog  implements Runnable{

    private PrintPreferences _parent;
    
    public RunPageSetupDialog(PrintPreferences parent){
	_parent = parent;
    }
    
    public void run() {
	_parent.pageFormat = _parent.printerJob.pageDialog(_parent.pageFormat);
	_parent.updateDialog();
    }
}

class RunPrinterSetupDialog  implements Runnable{

    private PrintPreferences _parent;
    
    public RunPrinterSetupDialog(PrintPreferences parent){
	_parent = parent;
    }
    
    public void run() {
	_parent.printerJob.printDialog();
	_parent.pageFormat = _parent.printerJob.validatePage(_parent.pageFormat);
	_parent.updateDialog();
    }
}
