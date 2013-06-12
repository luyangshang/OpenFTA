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

public class FTAList extends JPanel {

    private Container parentContainer;
    private JList pedList;
    private JScrollPane pedListScrollPane;
    private DefaultListModel pedListModel;
    private PEDFrame parentFrame;

    public FTAList(Container container, PEDFrame testFrame){

	parentContainer = container;
	parentFrame = testFrame;

	pedListScrollPane = new JScrollPane();

	pedListScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
	pedListScrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	
	pedListModel = new DefaultListModel();
	pedList = new JList(pedListModel);

	pedList.addListSelectionListener(new ListSelectionListener() {
		public void valueChanged(ListSelectionEvent e) {
		    parentFrame.showSelection();
		}
	    });

	JViewport vp = new JViewport();
	vp.setView(pedList);
	pedListScrollPane.setViewport(vp);

	setLayout(new GridLayout(1,1));
	setBorder(BorderFactory.createTitledBorder("Primary Events"));

	add(pedListScrollPane);
    }


    public int getWidth(){
    	return parentContainer.getWidth()/3;
    }

    public int getHeight(){
    	return parentContainer.getHeight();
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

    public void setSelectedValue(Object anObject, boolean shouldScroll){
	pedList.setSelectedValue(anObject, shouldScroll);
    }

    public void addElement(String obj){
    if (obj != null) {
        if(pedListModel.getSize() > 0) {
            //create an array to hold the existing and new elements
            String [] pedArray = new String [pedListModel.size()+1];
            pedListModel.copyInto(pedArray);
            //Add the element and sort the array
            pedArray[pedArray.length-1] = obj;
            Arrays.sort(pedArray); 
            //Clear the model
            pedListModel.clear(); 
            //Copy the sorted elements into the list
	        for (int i = 0; i<pedArray.length; i++) {
	            pedListModel.addElement(pedArray[i]);
	        }
        } else {
            //If it is the first element merely add it to the list
            pedListModel.addElement(obj);
        }
     }
    }

    public void clear() {
	pedListModel.clear();
    }

    public int listLength() {
	//return 1;
		return pedListModel.size();
    }

    public Object getSelectedValue() {
	return pedList.getSelectedValue();
    }
    public boolean contains(Object elem){
	return pedListModel.contains(elem);
    }

    public void clearSelection() {
	pedList.clearSelection();
    }

    public int getLastVisibleIndex() {
	return pedList.getSelectedIndex();
    }

    public void setSelectedIndex(int index) {
	pedList.setSelectedIndex(index);
    }

    public void selectPrimaryEvent(String id){
	int i=0;
	boolean found = false;
	/* loop through items in list until correct one found */

	while(!found && i < pedList.getModel().getSize()) {
	    if(id.equals(pedList.getModel().getElementAt(i))) {
		found = true;
	    } else {
		i++;
	    }
	}
	if(found) {
	    pedList.setSelectedIndex(i);
	} else {
	    pedList.clearSelection();
	}
    }

}

