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
import java.awt.*;
import java.awt.image.*;
import java.io.*;

public class FTAUtilities {

    public static JButton createButton(String name) {
	JarResources jar = new JarResources("images.jar");
	ImageIcon icon = new ImageIcon(
            Toolkit.getDefaultToolkit().createImage (jar.getResource(name + ".gif")), name);
	JButton b = new JButton(icon);
	b.setToolTipText(name);
	b.setMargin(new Insets(0,0,0,0));
	b.getAccessibleContext().setAccessibleName(name);
	return b;
    }


    public static ImageIcon loadImageIcon(String filename, String description) {
	JarResources jar = new JarResources("images.jar");
	return new ImageIcon(
            Toolkit.getDefaultToolkit().createImage (jar.getResource(filename)), description);
    }

    public static ImageIcon loadImageIcon(String filename) {
	JarResources jar = new JarResources("images.jar");
	return new ImageIcon(
            Toolkit.getDefaultToolkit().createImage (jar.getResource(filename)));
    }

    public static Image loadImage(String filename) {
	JarResources jar = new JarResources("images.jar");
	return Toolkit.getDefaultToolkit().createImage(jar.getResource(filename));
    }

    public static Image fillImageColour(Image image, Color penColour, Color fillColour, Color backColour) {
	    int i,j,r,g,b,a;
	    int[] imagePixels = new int[32*32];

	    PixelGrabber pg = new PixelGrabber(image, 0, 0, 32, 32, imagePixels, 0, 32);

	    // get pixels from mask
	    try {
		pg.grabPixels();
	    } catch (InterruptedException e) {
            //System.err.println("interrupted waiting for pixels!");
		return null;
	    }

	    // replace every black pixel with the pen colour
	    // replace every green pixel with the background colour
	    // replace every red pixel with the fill colour
	    for (j = 0; j < 32; j++) {
		for (i = 0; i < 32; i++) {
		    // get red, green, blue and alpha values
		    a = (imagePixels[j*32 + i] >> 24) & 0xff;
		    r = (imagePixels[j*32 + i] >> 16) & 0xff;
		    g = (imagePixels[j*32 + i] >> 8) & 0xff;
		    b = imagePixels[j*32 + i] & 0xff;

		    //System.out.println("mask pixels [" + i + ", " + j + "] = (" + r + ", " + g + ", " + b + ", " + a + ")");

		    if(r == 0 && g == 0 && b == 0 && a == 255) {
			// if pixel is black then change to pen colour
			imagePixels[j*32 + i] = penColour.getRGB();
		    } else if(r == 0 && g == 255 && b == 0 && a == 255) {
			// if pixel is green then change to background colour
			imagePixels[j*32 + i] = backColour.getRGB();
		    } else if(r == 255 && g == 0 && b == 0 && a == 255) {
			// if pixel is red then change to fill colour
			imagePixels[j*32 + i] = fillColour.getRGB();
		    }

		}
	    }

	    return Toolkit.getDefaultToolkit().createImage(new MemoryImageSource(32, 32, imagePixels, 0, 32));
    }

    public static int max(int a, int b) {
	if(a > b) {
	    return a;
	} else {
	    return b;
	}
    }

    // reduces the priority of a thread to make it play nicer on Windows.
    public static void setThreadPriority(Thread t){
	try {
	    t.setPriority(t.getPriority() - 1);	    
	} catch (java.lang.IllegalArgumentException x) {
	    // no need to do anything, just put up with higher priority
	} catch (java.lang.SecurityException x) {
	    // no need to do anything, just put up with higher priority
	}	
    }


}
