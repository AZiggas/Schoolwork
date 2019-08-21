/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7;

/**
 *
 * @author Andrew
 */

import java.io.*;
import java.util.*;
import java.lang.*;
import java.net.URL;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.TimeUnit;



public class Lab7 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try{
            //Take input from the user and set it to url.
            Scanner scan = new Scanner(System.in);
            URL inputUrl = new URL(scan.next());
            
            //This is a list set up for the sites given from robots.txt
            HashSet<String> disallowed = new HashSet<>();
            try{
                URL robots = new URL(inputUrl.toString() + "/robots.txt");
                InputStream inStream = robots.openStream();
                Scanner s = new Scanner(inStream);

                while(s.hasNextLine()){
                    String line = s.nextLine();
                    if(line.startsWith("Disallow:")){
                        Scanner s2 = new Scanner(line);
                        s2.next();
                        String forbidden = s2.next();
                        disallowed.add(forbidden);
                    }
                }
            }catch(Exception err){
                System.out.println("YOU DUN GOOFED AT ROBOTS STUFF: " + err);
            }

            //Make HashSet for visited and TreeMap for results

            HashSet<String> visited = new HashSet<>();
            TreeMap<String, ArrayList<String>> results = new TreeMap<>();

            ForkJoinPool pool = new ForkJoinPool();
            Runnable runnable = new Worker(inputUrl, disallowed, visited, results, pool, 0);
            pool.submit(runnable);
            pool.awaitQuiescence(3, TimeUnit.DAYS);
            pool.shutdown();
            pool.awaitTermination(30, TimeUnit.SECONDS);;
            
            //Set up out.txt
            PrintWriter write = new PrintWriter("out.txt", "UTF-8");
            
            //Going through the maps and stuff to make the out.txt
            Set set = results.entrySet();
            Iterator iterate = set.iterator();
            
            while(iterate.hasNext()){
				Map.Entry mEntry = (Map.Entry)iterate.next();
                
                //Where we write to out.txt
                write.println(mEntry.getKey() + ": " + mEntry.getValue());
            }
            write.close();
            
        }catch(Exception e){
            System.out.println("YOU DUN GOOFED AT URL STUFF: " + e);
        }        
    }
}
