package FTAGUI;

import java.awt.*;
import javax.swing.*;
import java.awt.print.*;
import java.awt.geom.*;

/** A simple utility class that lets you very simply print
 *  an arbitrary component. Just pass the component to the
 *  PrintUtilities.printComponent. The component you want to
 *  print doesn't need a print method and doesn't have to
 *  implement any interface or do anything special at all.
 *  <P>
 *  If you are going to be printing many times, it is marginally more 
 *  efficient to first do the following:
 *  <PRE>
 *    PrintUtilities printHelper = new PrintUtilities(theComponent);
 *  </PRE>
 *  then later do printHelper.print(). But this is a very tiny
 *  difference, so in most cases just do the simpler
 *  PrintUtilities.printComponent(componentToBePrinted).
 *
 *  7/99 Marty Hall, http://www.apl.jhu.edu/~hall/java/
 *  May be freely used or adapted.
 */

public class PrintUtilities implements Printable, Pageable {
    private Component componentToBePrinted;
    private int numPagesX;
    private int numPagesY;
    private PageFormat pf;

    public static void printComponent(Component c, int npx, int npy, PrinterJob printJob, PageFormat pageFormat) {
	new PrintUtilities(c, npx, npy).print(printJob, pageFormat);
    }
  
    public PrintUtilities(Component componentToBePrinted, int npx, int npy) {
	this.componentToBePrinted = componentToBePrinted;
	this.numPagesX = npx;
	this.numPagesY = npy;
    }
  
    public void print(PrinterJob printJob, PageFormat pageFormat) {

	pf = pageFormat;
	
	if (printJob.printDialog()) {
	    try {
		//		printJob.pageDialog(pageFormat);
		printJob.setPageable(this);
		printJob.print();
	    } catch(PrinterException pe) {
		System.out.println("Error printing: " + pe);
	    }
	}
    }

    public int print(Graphics g, PageFormat pageFormat, int pageIndex) {
	if (pageIndex > (numPagesX*numPagesY-1)) {
	    return(NO_SUCH_PAGE);
	} else {
	    Graphics2D g2d = (Graphics2D)g;
	    double width_ratio;
	    double height_ratio;
	    double paper_width;
	    double paper_height;
	    double tx;
	    double ty;

	    int i,j;
	    int sectionWidth;
	    int sectionHeight;

	    //System.out.println("Page Number = " + (pageIndex + 1));
          
          // calculate page indicies
	    j = pageIndex / numPagesX;
	    i = pageIndex - (j*numPagesX);

          //sectionWidth = componentToBePrinted.getWidth() / numPagesX;
          //sectionHeight= componentToBePrinted.getHeight()  / numPagesY;

	    //width_ratio = pageFormat.getImageableWidth()/sectionWidth;
	    //height_ratio = pageFormat.getImageableHeight()/sectionHeight;

	    width_ratio = (double)(numPagesX * pageFormat.getImageableWidth())/(double)componentToBePrinted.getWidth();
          height_ratio = (double)(numPagesY * pageFormat.getImageableHeight())/(double)componentToBePrinted.getHeight();


          //if(pageFormat.getOrientation() == PageFormat.PORTRAIT) {
          //System.out.println("Paper format = PORTRAIT");
          //} else {
          //System.out.println("Paper format = LANDSCAPE");
          //}

	    // calculate page indicies
	    j = pageIndex / numPagesX;
	    i = pageIndex - (j*numPagesX);

	    //System.out.println("i = " + i);
	    //System.out.println("j = " + j);

	    // Move the origin on the paper to rest on the origin of the actual drawing area
	    g2d.translate(pageFormat.getImageableX(), pageFormat.getImageableY());

	    //System.out.println("Component width = " + componentToBePrinted.getWidth());
	    //System.out.println("Component height = " + componentToBePrinted.getHeight());
	    //System.out.println("Section width = " + sectionWidth);
	    //System.out.println("Section height = " + sectionHeight);
	    //System.out.println("Paper width = " + pageFormat.getImageableWidth());
	    //System.out.println("Paper height = " + pageFormat.getImageableHeight());
	    //System.out.println("Width ratio = " + width_ratio);
	    //System.out.println("Height ratio = " + height_ratio);

	    // use smallest ratio
	    if(width_ratio < height_ratio) {
		g2d.scale(width_ratio, width_ratio);
        //System.out.println("Component height is " + componentToBePrinted.getHeight());
        //System.out.println("component width is " + componentToBePrinted.getWidth()); 
            //sectionHeight = (int)((sectionWidth * sectionWidth) / pageFormat.getImageableHeight());
            sectionWidth = (int)((float)componentToBePrinted.getWidth() / (float)numPagesX);
		sectionHeight = (int)(((float)sectionWidth /(float)pageFormat.getImageableWidth()) * (float)pageFormat.getImageableHeight());
        //System.out.println(sectionHeight);
          } else {
	    	g2d.scale(height_ratio, height_ratio);
            //System.out.println("Component width is: "  + componentToBePrinted.getWidth());
            //sectionWidth = (int)((sectionHeight * sectionHeight) / pageFormat.getImageableWidth());
            sectionHeight = (int)((float)componentToBePrinted.getHeight() / (float)numPagesY);
		sectionWidth = (int)((float)sectionHeight / (float)pageFormat.getImageableHeight() * (float)pageFormat.getImageableWidth());
           
	    }

	    //System.out.println("Section width = " + sectionWidth);
	    //System.out.println("Section height = " + sectionHeight);

	    // set translation required to print current section
	    tx = -i*sectionWidth;
	    ty = -j*sectionHeight;
	    //System.out.println("tx = " + tx);
	    //System.out.println("ty = " + ty);
	    g2d.translate(tx, ty);

	    disableDoubleBuffering(componentToBePrinted);
	    componentToBePrinted.paint(g2d);
	    enableDoubleBuffering(componentToBePrinted);

	    return(PAGE_EXISTS);
	}
    }

    /** The speed and quality of printing suffers dramatically if
     *  any of the containers have double buffering turned on.
     *  So this turns if off globally.
     *  @see enableDoubleBuffering
     */
    public static void disableDoubleBuffering(Component c) {
	RepaintManager currentManager = RepaintManager.currentManager(c);
	currentManager.setDoubleBufferingEnabled(false);
    }

    /** Re-enables double buffering globally. */
  
    public static void enableDoubleBuffering(Component c) {
	RepaintManager currentManager = RepaintManager.currentManager(c);
	currentManager.setDoubleBufferingEnabled(true);
    }

    public int getNumberOfPages() {
	return numPagesX*numPagesY;
    }

    public PageFormat getPageFormat(int pageIndex)
	throws IndexOutOfBoundsException {
	
	if(pageIndex >= getNumberOfPages()) {
	    throw new IndexOutOfBoundsException("Page " + (pageIndex+1) + "does not exist");
	} 
	return pf;
    }

    public Printable getPrintable(int pageIndex)
	throws IndexOutOfBoundsException {

	if(pageIndex >= getNumberOfPages()) {
	    throw new IndexOutOfBoundsException("Page " + (pageIndex+1) + "does not exist");
	} 
	
	return this;
    }


}
