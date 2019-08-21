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
				BufferedChannel<String> readerOk = new BufferedChannel<>(20);
				BufferedChannel<Tuple> decoderUnread = new BufferedChannel<>(20);
				BufferedChannel<Tuple> decoded = new BufferedChannel<>(20);
				
				BufferedChannel<Tuple> compare = new BufferedChannel<>(20);
				
				BufferedChannel<Tuple> output = new BufferedChannel<>(20);
				
				File file = choose.getSelectedFile();
				//System.out.println("You chose to open this file: " + file.getName() + ".\n");
				ArrayList<Thread> Threads = new ArrayList<>();
				Threads.add(new Thread(new Reader(file, 4, readerOk,decoderUnread)));
				Threads.add(new Thread(new Decoder(decoderUnread, decoded)));
				Threads.add(new Thread(new Decoder(decoderUnread, decoded)));
				Threads.add(new Thread(new Decoder(decoderUnread, decoded)));
				Threads.add(new Thread(new Decoder(decoderUnread, decoded)));
				Threads.add(new Thread(new Stitcher(2,4, decoded, compare)));
				Threads.add(new Thread(new Comparator(compare, output, readerOk)));
				Threads.add(new Thread(new Comparator(compare, output, readerOk)));
				Threads.add(new Thread(new Output(2, output)));
				
				for(int i = 0; i < 20; i++){
					readerOk.put("OK");
				}
				
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