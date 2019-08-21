import java.net.*;
import java.io.*;
import java.util.*;
import java.util.concurrent.locks.*;
import java.awt.*;
import javax.swing.*;

class tClass implements Runnable{
	
	final Lock lock;
	ArrayList<String> list;
	ArrayList<tClass> cList;
	String str;
	boolean active;
		
	tClass(ArrayList<String> list, ArrayList<tClass> cList, Lock lock, String str){
		this.list = list;
		this.lock = lock;
		this.str = str;
		this.active = true;
		this.cList = cList;
	}
	
	
	public void run(){
		while(active){
			lock.lock();
			String urlToUse;
			try{
				if(list.size() == 0){
					this.active = false;
					
					boolean flag = false;
					for(tClass t : cList){
						if(t.active == true){
							flag = true;
						}
					}
					if(flag == false){
						System.exit(0);
					}
					
					break;
				}
				urlToUse = list.get(0);
				//System.out.println(urlToUse);
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
	}

	public void setFlag(int bool){
		if(bool == 1){
			this.active = true;
		}
		else if(bool == 0){
			this.active = false;
		}
	}
	
}


class kClass implements Runnable{
	Scanner scan;
	ArrayList<String> list;
	ArrayList<tClass> cList;
	Lock lock;
	String str;
	int index;
	
	kClass(ArrayList<String> list, ArrayList<tClass> cList, String str, Lock lock){
		this.scan = new Scanner(System.in);
		this.list = list;
		this.cList = cList;
		this.index = cList.size();
		this.str = str;
		this.lock = lock;
	}
	
	
	public void run(){
		scan = new Scanner(System.in);
		while(list.size() > 0){
			if(scan.hasNextLine()){
				String input = scan.next();
				if((input.charAt(0) == 'm' || input.charAt(0) == 'M') && index < 33){
					if(index == cList.size()){
						tClass temp = new tClass(list, cList, lock, str);
						cList.add(temp);
						Thread temps = new Thread(temp);
						temps.start();
						index++;
						
					}
					else if(index < cList.size() && cList.size() > 0){
						index++;
						cList.get(index-1).setFlag(1);
					}
					else if(cList.isEmpty()){
						tClass temp = new tClass(list, cList, lock, str);
						cList.add(temp);
						Thread temps = new Thread(temp);
						temps.start();
						index++;
					}
				}
				else if((input.charAt(0) == 'l' || input.charAt(0) == 'L') && index > 0){
					cList.get(index-1).setFlag(0);
					index--;
				}
			}
		}
	}
}

public class Lab4{
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
		
		ArrayList<tClass> cList = new ArrayList<>();
		
		final Lock lock = new ReentrantLock();
		
		Thread temps = new Thread(new kClass(list,cList,str,lock));
		temps.start();
		
		for(int i = 0; i < 4; i++){
			tClass temp = new tClass(list, cList, lock, str);
			cList.add(temp);
			Thread tmp = new Thread(temp);
			tmp.start();
		}
	}	
}