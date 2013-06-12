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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import javax.swing.*;

public class BrowserLauncher {

    static void openURL (String url) {

	String command = Preferences.browserLocation + " " + url;

	try {
	    Runtime.getRuntime().exec(command);
	} catch (IOException ioe) {
	    String message = new String("Problem launching Web Browser using command:\n"
					+  Preferences.browserLocation + "\nCheck Preferences");


	    JOptionPane.showMessageDialog(null, message, "Web Browser Error",
					  JOptionPane.WARNING_MESSAGE, null);
	}

    }

    static void openHelpFile (String helpFile) {

	try {
	    // convert file path to a URL with no spaces
	    File urlFile = new File(AppLocation.getHelpLocation() + File.separator + helpFile);
	    String urlString = urlFile.toURL().toString();
	    urlString = urlString.replaceAll(" ", "%20");
	    BrowserLauncher.openURL(urlString);
	} catch (java.net.MalformedURLException mue) {
	    String message = new String("Problem opening Help File: " + helpFile);

	    JOptionPane.showMessageDialog(null, message, "URL Error",
					  JOptionPane.WARNING_MESSAGE, null);
	}
    }

}
