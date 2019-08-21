import java.io.*;
//import java.lang.*;

public class Output implements Runnable{
	int numComparators;
	int doneCount;
	
	Output(int numComparators){
		this.numComparators = numComparators;
		this.doneCount = 0;
	}
	@Override
	public void run(){
		try{
			PrintWriter writer = new PrintWriter("out.txt", "UTF-8");
			while(true){
				Tuple t = Blackboard.get("Out","ComparatorDone");
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