import java.util.*;
import java.io.*;
import java.math.*;
import java.lang.*;

public class Lab1{
	public static void main(String[] args){
		
		try{
			if(args.length > 0 && args[0].equals("child")){
				
				BigDecimal out = new BigDecimal(0.0);
				
				for(int k = Integer.parseInt(args[1]); k < Integer.parseInt(args[2]); k++){
					
					BigDecimal t1 = new BigDecimal(16).pow(k);
					double t2 = 4.0/((8.0*k) + 1.0);
					double t3 = 2.0/((8.0*k) + 4.0);
					double t4 = 1.0/((8.0*k) + 5.0);
					double t5 = 1.0/((8.0*k) + 6.0);
					BigDecimal bDec1 = new BigDecimal(t2-t3-t4-t5);
					bDec1 = bDec1.divide(t1);
					out = out.add(bDec1);
				}
				
				System.out.println(out);
				
				
			}else{
				//Make a list of all processes.
				ArrayList<Process> processList = new ArrayList<>();
				BigDecimal results = new BigDecimal(0.0);
				
				//cores
				int cores = Integer.parseInt(args[0]);
				//Digits of pi
				int index = Integer.parseInt(args[1]);
				
				
				double start = System.nanoTime();
				
				for(int i = 0; i < cores; i++){
					processList.add(i, new ProcessBuilder("java", "Lab1", "child", Integer.toString((i)*index/cores), Integer.toString((i+1)*index/cores)).start());
				}
				
				for(int k = 0; k < cores; k++){
					String res = new Scanner(processList.get(k).getInputStream()).nextLine();
					results = results.add(new BigDecimal(res));
					
				}
				
				double end = System.nanoTime();
				
				double total = end - start;
				
				total = (double)(total*0.000000001);
				
				
				System.out.println("Result:" + total + " seconds");
				System.out.println(results);
			}
		}catch(IOException e){
			
		}catch(NoSuchElementException err){
			
		}
	}
}
