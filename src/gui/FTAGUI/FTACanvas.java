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

import com.sun.image.codec.jpeg.*;

public class FTACanvas extends JPanel implements MouseListener{

    private Graphics2D gRef;
    private FontMetrics fmRef;
    private FTAFrame parentFTAFrame;
    private Container parentContainer;
    private int maxHeight;
    private int maxWidth;

    private TreeTextDialog treeTextWindow;
    private IntermediateEventDialog intermediateEventWindow;
    private GateDialog gateWindow;
    private TransferTreeDialog transferTreeWindow;

    private native void nativeInitialiseFTACanvas();
    private native void nativeDrawTree();
    private native boolean nativeSelectItem(int x, int y);
    public native int nativeGetMaxWidth();
    public native int nativeGetMaxDepth();
    private native void nativeEditSelected();

    private float scale;
    private boolean focusEventsEnabled;
    private boolean textEnabled;

    private Color penColor;

    private boolean mouseEventsEnabled;

    private int margin = 10;

    public FTACanvas(Container container, Frame frame) {

	super();

	scale = 1.0f;
	focusEventsEnabled = false;
	mouseEventsEnabled = true;
	textEnabled = true;

	parentContainer = container;

	addMouseListener(this);
	setBackground(Color.white);
	setAutoscrolls(true);
	setDoubleBuffered(true);
	nativeInitialiseFTACanvas();

	treeTextWindow = new TreeTextDialog(frame);
	intermediateEventWindow = new IntermediateEventDialog(frame);
	gateWindow = new GateDialog(frame);
	transferTreeWindow = new TransferTreeDialog(frame);
    }

    public void setFTAFrame(FTAFrame fta) {
	parentFTAFrame = fta;
    }

    public void setFocusEventsEnabled(boolean enabled){
	focusEventsEnabled = enabled;
	
    }


    public void setMouseEventsEnabled(boolean enabled){
	mouseEventsEnabled = enabled;
	
    }


    public void paint (Graphics g) {
	gRef = (Graphics2D)g;
	
	// change size of font
	gRef.setFont(gRef.getFont().deriveFont(9.0f));

	fmRef = g.getFontMetrics();


	// Clear background

	if(Preferences.monochrome) {
	    gRef.setColor(Preferences.whiteColor);
	} else {
	    gRef.setColor(Preferences.backgroundColor);
	}
	gRef.fillRect(0, 0, getWidth(), getHeight());

	// set colour to correct drawing colour
	if(Preferences.monochrome) {
	    gRef.setColor(Preferences.blackColor);
	} else {
	    gRef.setColor(Preferences.penColor);
	}

	gRef.translate(0, margin);

	// Call c code to draw tree
	gRef.scale(scale, scale);
	nativeDrawTree();
    }

    public void setScale(float s){
	if(scale > 0.0f) {
	    scale = s;
	}
    }

    public float getScale(){
	return scale;
    }

    public void zoomIn(){
	scale += 0.1f;
    }

    public void zoomOut(){
	scale -= 0.1f;
	if(scale < 0.1f) {
	    scale = 0.1f;
	}
    }

    public int getWidth(){
	float fWidth;
	int width;
	fWidth = scale*nativeGetMaxWidth();
	width = Math.round(fWidth);
	if(parentContainer == null) {
	    return width;
	} else {
	    return FTAUtilities.max(width, parentContainer.getWidth());
	}
    }

    public int getHeight(){
	float fHeight;
	int height;
	fHeight = scale*nativeGetMaxDepth();
	height = Math.round(fHeight) + (2*margin);
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

    private void setFillColor(int symbolType) {
	penColor = gRef.getColor();
	switch(symbolType) {

	case FTAConstants.SYMB_BASIC:
	    gRef.setColor(Preferences.basicInitiatingEventColor);
	    break;
	case FTAConstants.SYMB_UNDEV:
	    gRef.setColor(Preferences.undevelopedEventColor);
	    break;
	case FTAConstants.SYMB_EXTERN:
	    gRef.setColor(Preferences.externalEventColor);
	    break;
	case FTAConstants.SYMB_INTERMEDIATE:
	    gRef.setColor(Preferences.intermediateEventColor);
	    break;
	case FTAConstants.SYMB_AND:
	    gRef.setColor(Preferences.andGateColor);
	    break; 
	case FTAConstants.SYMB_OR:
	    gRef.setColor(Preferences.orGateColor);
	    break; 
	case FTAConstants.SYMB_PRIORITY_AND:
	    gRef.setColor(Preferences.priorityAndGateColor);
	    break; 
	case FTAConstants.SYMB_EXCL_OR: 
	    gRef.setColor(Preferences.exclusiveOrGateColor);
	    break; 
	case FTAConstants.SYMB_INHIBIT:
	    gRef.setColor(Preferences.inhibitGateColor);
	    break;
	case FTAConstants.SYMB_CONDITION:
	    gRef.setColor(Preferences.conditioningEventColor);
	    break;
	case FTAConstants.SYMB_TRANS_IN:
	    gRef.setColor(Preferences.transferInColor);
	    break;
	case FTAConstants.SYMB_TRANS_OUT:
	    gRef.setColor(Preferences.transferOutColor);
	    break;
	default:
	    gRef.setColor(Preferences.backgroundColor);
	    break;
	}

    }

    private void restoreColor() {
	gRef.setColor(penColor);
    }

    private void fillRectangle(int x, int y, int width, int height, int symbolType) {
	if(!Preferences.monochrome) {
	    setFillColor(symbolType);
	    gRef.fillRect(x, y, width, height);
	    restoreColor();
	}
    }

    private void drawRectangle(int x, int y, int width, int height) {
	gRef.drawRect(x, y, width, height);
    }

    private void clearArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
	penColor = gRef.getColor();
	gRef.setColor(Preferences.backgroundColor);
	gRef.fillArc(x, y, width, height, startAngle, arcAngle);
	gRef.setColor(penColor);
    }

    private void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle, int symbolType) {
	if(!Preferences.monochrome) {
	    setFillColor(symbolType);
	    gRef.fillArc(x, y, width, height, startAngle, arcAngle);
	    restoreColor();
	}
    }

    private void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
	gRef.drawArc(x, y, width, height, startAngle, arcAngle);
    }

    private void fillPolyline(int x[], int y[], int length, int symbolType) {
	if(!Preferences.monochrome) {
	    setFillColor(symbolType);
	    gRef.fillPolygon(x, y, length);
	    restoreColor();
	}
    }

    public void drawLine(int x1, int y1, int x2, int y2) {
	gRef.drawLine(x1, y1, x2, y2);
    }

    private void drawString(String str, int x, int y) {

	// text does not work correctly for very small scales.
	if(textEnabled && scale > 0.05) {
	    gRef.drawString(str, x, y);
	}
    }

    public void setPenColour(int colour) {

	if(Preferences.monochrome) {
	    gRef.setColor(Preferences.blackColor);
	} else {
	    if(colour==0){
		gRef.setColor(Preferences.penColor);
	    }
	    if(colour==1){
		gRef.setColor(Preferences.highlightColor);
	    }
	}
    }

    private void setPenWidth(int width) {
	    gRef.setStroke(new BasicStroke(width));
    }

    private int getTextWidth(String text){
	return fmRef.stringWidth(text);
    }

    private int getFontHeight(){
	return fmRef.getHeight();
    }

    // MouseListener methods

    public void mousePressed(MouseEvent e) {
    }

    public void mouseReleased(MouseEvent e) {
    }

    public void mouseEntered(MouseEvent e) {
    }

    public void mouseExited(MouseEvent e) {
    }

    public void mouseClicked(MouseEvent e) {
	int x;
	int y;

	e.consume();

	if(mouseEventsEnabled) {

	    x=Math.round(e.getX()/scale);
	    y=Math.round(e.getY()/scale);
	    
	    // allow for the canvas margin
	    y -= margin;

	    //System.out.println("Mouse Click: (" + x + ", " + y + ")");

	    if(e.getClickCount() < 2) {
		if(nativeSelectItem(x, y)) {
		    parentFTAFrame.updateFrame();
		}
	    } else {
		if(nativeSelectItem(x, y)) {
		    parentFTAFrame.updateFrame();
		}
		
		editSelected();
		parentFTAFrame.updateFrame();
	    }
	    
	    if(focusEventsEnabled) {
		// tell the main Canvas to point to this coordinate
		parentFTAFrame.setCanvasFocus(x,y);
	    }
	}
    }

    public void editSelected(){
	nativeEditSelected();
    }

    private void editTreeText(){
	treeTextWindow.initialise();
	treeTextWindow.setVisible(true);
    }
	
    private void editIntermediateEvent(){
	intermediateEventWindow.initialise();
	intermediateEventWindow.setVisible(true);
    }

    private void editGate(){
	gateWindow.initialise();
	gateWindow.setVisible(true);
    }

    private void editTransferTree(){
	transferTreeWindow.initialise();
	transferTreeWindow.setVisible(true);
    }

    public void setTextEnabled(boolean te){
	textEnabled = te;
    }

    public void selectPrimaryEvent(String id){
	parentFTAFrame.selectPrimaryEvent(id);
    }

}

