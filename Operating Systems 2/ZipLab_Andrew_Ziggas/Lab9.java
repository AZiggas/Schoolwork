import java.util.*;
import javax.swing.JFileChooser;
import java.io.*;


//Probably have a decoder and comparator that will also print.
public class Lab9 {
    public static void main(String[] args) {
		JFileChooser choose = new JFileChooser();
		choose.setCurrentDirectory(new File("."));
		int returnVal = choose.showOpenDialog(null);
		if(returnVal == JFileChooser.APPROVE_OPTION){
			try{
				File file = choose.getSelectedFile();
				//System.out.println("You chose to open this file: " + file.getName() + ".\n");
				ArrayList<Thread> Threads = new ArrayList<>();
				Threads.add(new Thread(new Reader(file, 4)));
				Threads.add(new Thread(new Decoder(0,4)));
				Threads.add(new Thread(new Decoder(1,4)));
				Threads.add(new Thread(new Decoder(2,4)));
				Threads.add(new Thread(new Decoder(3,4)));
				Threads.add(new Thread(new Comparator(2,4,0)));
				Threads.add(new Thread(new Comparator(2,4,1)));
				Threads.add(new Thread(new Output(2)));
				
				for(int i = 0; i < 20; i++){
					Blackboard.put("OK");
				}
				Blackboard.put("doneCount", 0);
				
				for(Thread t : Threads){
					t.start();
				}
				for(Thread j : Threads){
					j.join();
				}
			}catch(InterruptedException IE){
				System.out.println(IE);
			}
		}
		//Format for jpg files is YYYY.MM.DD.HH:MM:SS.FFF.jpg
	}
}

//Tuplespace is whiteboard. Tuple is a post-it note.