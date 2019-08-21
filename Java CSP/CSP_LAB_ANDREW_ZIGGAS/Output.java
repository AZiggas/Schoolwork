import java.io.*;
//import java.lang.*;

public class Output implements Runnable{
	int numComparators;
	int doneCount;
	BufferedChannel<Tuple> output;
	
	Output(int numComparators, BufferedChannel<Tuple> output){
		this.numComparators = numComparators;
		this.output = output;
		this.doneCount = 0;
	}
	@Override
	public void run(){
		try{
			PrintWriter writer = new PrintWriter("out.txt", "UTF-8");
			while(true){
				Tuple t = output.get();
				if(t.tag.equals("ComparatorDone")){
					doneCount = doneCount + 1;
					if(doneCount >= numComparators){
						break;
					}
				}
				else{
					System.out.println("WRITING!!!!");
					writer.println(t.data[0] + " and " + t.data[1]);
					writer.flush();
				}
			}
			writer.close();
		}catch(IOException IOE){
			System.out.println(IOE);
		}
	}
}