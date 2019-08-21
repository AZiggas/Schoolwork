import java.net.*;
import java.io.*;
import java.util.*;
import java.util.concurrent.locks.*;
import java.awt.*;
import javax.swing.*;

public class Lab3{
	public static void main(String[] args) throws FileNotFoundException{
		
		JFileChooser jf = new JFileChooser(".");
		
		if( jf.showOpenDialog(null) != JFileChooser.APPROVE_OPTION)
			System.exit(0);
		
		File file = jf.getSelectedFile();
		String str = file.getParent();
		System.out.println(str);
		
		final ArrayList<String> list = new ArrayList<>();
		Scanner s = new Scanner(jf.getSelectedFile());
		
		while(s.hasNextLine()){
			list.add(s.nextLine());
		}
		
		ArrayList<Thread> tList = new ArrayList<>();
		
		final Lock lock = new ReentrantLock();
		for(int i = 0; i < 4; i++){
			//tClass t = new tClass(list, str);
			
			Thread temp = new Thread(() -> {
				
				while(true){
					lock.lock();
					String urlToUse;
					try{
						if(list.size() == 0) break;
						urlToUse = list.get(0);
						System.out.println(urlToUse);
						list.remove(0);
					}finally{
						lock.unlock();
					}
					
					try{
						// Here is where you download content of urlToUse
						URL thing = new URL(urlToUse);
						InputStream in = thing.openStream();
						
						String url = thing.toString();
						//Anything but letters numbers and dots will be underscores.
						String name = url.substring(url.lastIndexOf("/")+1,url.length());
						name = name.replaceAll("[^a-zA-z0-9\\.]", "_");
						
						File blah = new File(str + "\\" + name);
						
						OutputStream out = new FileOutputStream(blah);
						
						byte[] b = new byte[4096];
						
						int readCheck;
						while((readCheck = in.read(b)) != -1){
							out.write(b, 0, readCheck);
						}
						
						out.close();
					}catch(Exception e){
						System.out.println(e);
					}
					
				}
			});

			temp.start();
			tList.add(temp);
		}
		
		for(Thread t : tList){
			t.join();
		}
	}
}