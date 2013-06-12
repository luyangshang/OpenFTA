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
import java.awt.print.*;
import java.awt.image.*;

public class PrintPreviewCanvas extends JPanel{

    private int numPagesX;
    private int numPagesY;
    private int onScreenPageWidth;
    private int onScreenPageHeight;
    private int zoom;
    private int pageSeperation;
    private int shadowSize = 4;
    private FTACanvas ftaCanvas;
    private Container parentContainer;

    private Image printPreviewImage;
    private boolean firstPaint;
    public PrintPreviewCanvas(int npx, int npy, int z , int ps, Container container) {
	super();
	
	firstPaint = true;
	numPagesX = npx;
	numPagesY = npy;
	zoom = z;
	setOnScreenPageDimensions();
	pageSeperation = ps;
	parentContainer = container;
	ftaCanvas = new FTACanvas(null, null);
    }
    
    public void setNumPagesX(int n) {
	numPagesX = n;
    }
    
    public void setNumPagesY(int n) {
	numPagesY = n;
    }

    public void setZoom(int z) {
	if(z > 0) {
	    zoom = z;
	    setOnScreenPageDimensions();
	}
    }
    public FTACanvas getFTACanvas() {
       return ftaCanvas;
    }
    
    private void setOnScreenPageDimensions() {
	onScreenPageWidth = (int)Math.round((float)zoom/100.0f * PrintPreferences.pageFormat.getWidth());
	onScreenPageHeight = (int)Math.round((float)zoom/100.0f * PrintPreferences.pageFormat.getHeight());
    }
    
    public void setPageSeperation(int ps) {
	pageSeperation = ps;
    }

    public void updatePrintPreviewImage() {
	Graphics2D g2d;
	BufferedImage ftaImage;
	BufferedImage tempImage;
	Graphics2D imageG2d;
	int i, j, x, y;
	int onScreenImageableWidth;
	int onScreenImageableHeight;
	int onScreenTopMargin;
	int onScreenLeftMargin;
	float widthRatio;
	float heightRatio;
	    
	// update paper dimensions
	setOnScreenPageDimensions();

	// create print preview image of correct size
	printPreviewImage = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_ARGB);
	g2d = (Graphics2D)printPreviewImage.getGraphics();


	// calculate on screen drawable paper sizes
	onScreenImageableWidth = (int)Math.round(onScreenPageWidth * PrintPreferences.pageFormat.getImageableWidth() / PrintPreferences.pageFormat.getWidth());
	onScreenImageableHeight = (int)Math.round(onScreenPageHeight * PrintPreferences.pageFormat.getImageableHeight() / PrintPreferences.pageFormat.getHeight());

	// calculate on sceen margins
	onScreenLeftMargin = (int)Math.round(onScreenPageWidth / PrintPreferences.pageFormat.getWidth() * PrintPreferences.pageFormat.getImageableX());
	onScreenTopMargin = (int)Math.round(onScreenPageHeight / PrintPreferences.pageFormat.getHeight() * PrintPreferences.pageFormat.getImageableY());

	if(getWidth() > 0 && getHeight() > 0) {
	    // find the scale required for both x and y directions
	    widthRatio = (float)(numPagesX * onScreenImageableWidth)/(float)ftaCanvas.nativeGetMaxWidth();
	    heightRatio = (float)(numPagesY * onScreenImageableHeight)/(float)ftaCanvas.nativeGetMaxDepth();
	    
	    //System.out.println("Width ratio = " + widthRatio);
	    //System.out.println("Height ratio = " + heightRatio);

	    // use the smallest scale to ensure that tree fits in window
	    if(widthRatio < heightRatio) {
            //System.out.println("scale (using width ratio) = " + widthRatio);
		ftaCanvas.setScale(widthRatio);
	    } else {
            //System.out.println("scale (using height ratio) = " + heightRatio);
		ftaCanvas.setScale(heightRatio);
	    }
	} else {
	    ftaCanvas.setScale(1.0f);
	}

	// create new image to hold tree rendering

	ftaImage = new BufferedImage(numPagesX * onScreenImageableWidth, numPagesY * onScreenImageableHeight, BufferedImage.TYPE_INT_ARGB
);
	imageG2d = (Graphics2D)ftaImage.getGraphics();
	
	// draw tree into image
	if(Preferences.monochrome) {
	    imageG2d.setColor(Preferences.whiteColor);
	} else {
	    imageG2d.setColor(Preferences.backgroundColor);
	}
	imageG2d.fillRect(0, 0, numPagesX * onScreenImageableWidth, numPagesY * onScreenImageableHeight);

	ftaCanvas.paint(imageG2d);

	// fill in background

	/*
	System.out.println("paperImageableWidth = " + PrintPreferences.pageFormat.getImageableWidth());
	System.out.println("paperImageableHeight = " + PrintPreferences.pageFormat.getImageableHeight());
	
	System.out.println("pageSeperation = " + pageSeperation);
	System.out.println("onScreenPageWidth = " + onScreenPageWidth);
	System.out.println("onScreenPageHeight = " + onScreenPageHeight);
	System.out.println("onScreenLeftMargin = " + onScreenLeftMargin);
	System.out.println("onScreenTopMargin = " + onScreenTopMargin);
	System.out.println("onScreenImageableWidth = " + onScreenImageableWidth);
	System.out.println("onScreenImageableHeight = " + onScreenImageableHeight);
	*/

	g2d.setColor(Preferences.printPreviewBackgroundColor);

	/*	
	System.out.println("width = " + getWidth());
	System.out.println("height = " + getHeight());
	System.out.println("container width = " + super.getWidth());
	System.out.println("container height = " + super.getWidth());
	*/

	g2d.fillRect(0, 0, getWidth(), getHeight());

	//g2d.drawImage(image, 0, 0, this);

	
	for(j = 0; j < numPagesY; j++) {
	    for(i = 0; i < numPagesX; i++) {

		x = pageSeperation + (i * (pageSeperation + onScreenPageWidth));
		y = pageSeperation + (j * (pageSeperation + onScreenPageHeight));

		/*
		System.out.println("x[" + i + ", " + j + "] = " + x);
		System.out.println("y[" + i + ", " + j + "] = " + y);
		*/

		// fill shadow
		g2d.setColor(Preferences.printPreviewShadowColor);
		g2d.fillRect(x + shadowSize, y + shadowSize, onScreenPageWidth, onScreenPageHeight);

		// fill page
		g2d.setColor(Preferences.printPreviewPageColor);
		g2d.fillRect(x, y, onScreenPageWidth, onScreenPageHeight);

		// place picture in correct place
		tempImage = ftaImage.getSubimage(i*onScreenImageableWidth, j*onScreenImageableHeight, onScreenImageableWidth, onScreenImageableHeight);
		g2d.drawImage(tempImage, x + onScreenLeftMargin, y + onScreenTopMargin, this); 

		// draw page outline
		g2d.setColor(Preferences.printPreviewPenColor);
		g2d.drawRect(x, y, onScreenPageWidth, onScreenPageHeight);

	    }
	}

    }


    public void paint (Graphics g) {
	Graphics2D g2d = (Graphics2D)g;

	if(firstPaint) {
	    updatePrintPreviewImage();
	    firstPaint = false;
	}

	// fill in background where canvas too large for image
	g2d.setColor(Preferences.printPreviewBackgroundColor);
	g2d.fillRect(0, 0, getWidth(), getHeight());

	// draw pre-prepared print preview image
	g2d.drawImage(printPreviewImage, 0, 0, this);

    }


    public int getWidth() {
	int width = (numPagesX * (onScreenPageWidth + pageSeperation)) + pageSeperation;
	if(parentContainer == null) {
	    return width;
	} else {
	    return FTAUtilities.max(width, parentContainer.getWidth());
	}
    }

    public int getHeight(){
	int height = (numPagesY * (onScreenPageHeight + pageSeperation)) + pageSeperation;
	if(parentContainer == null) {
	    return height;
	} else {
	    return FTAUtilities.max(height, parentContainer.getHeight());
	}
    }

    public Dimension getPreferredSize(){
	return new Dimension(getWidth(), getHeight());
    }

    public Dimension getMinimumSize(){
	return new Dimension(getWidth(), getHeight());
    }

    public Dimension getMaximumSize(){
	return new Dimension(getWidth(), getHeight());
    }


}

