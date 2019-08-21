
import java.io.*;
import java.util.*;
import java.net.URL;
import java.lang.*;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.TimeUnit;



public class Lab7 {
    public static void main(String[] args) {
        try{
            //Take input from the user and set it to url.
            Scanner scan = new Scanner(System.in);
            URL inputUrl = new URL(scan.next());
            
            //This is a list set up for the sites given from robots.txt
            HashSet<String> disallowed = new HashSet<>();
            try{
				//Robots.txt is a file with all of the urls we aren't allowed to crawl.
                URL robots = new URL(inputUrl.toString() + "/robots.txt");
                InputStream inStream = robots.openStream();
                Scanner s = new Scanner(inStream);

				//I'm not sure if there are any spaces after the url so I'm using another scanner to grab the url after disallow.
                while(s.hasNextLine()){
                    String line = s.nextLine();
                    if(line.startsWith("Disallow:")){
                        Scanner s2 = new Scanner(line);
                        s2.next();
                        String robotUrl = inputUrl + s2.next();
                        System.out.println(robotUrl);
						disallowed.add(robotUrl);
                    }
                }
            }catch(Exception err){
                System.out.println("YOU DUN GOOFED AT ROBOTS STUFF: " + err);
            }

            //Make HashSet for visited and TreeMap for results
            HashSet<String> visited = new HashSet<>();
			//Using the treemap to set words found to keys.
            TreeMap<String, ArrayList<String>> results = new TreeMap<>();

			//Jim said we should probably use something else besides the forkjoinpool due to possible bandwidth issues.
            ForkJoinPool pool = new ForkJoinPool();
            Runnable runnable = new Worker(inputUrl, disallowed, visited, results, pool, 0);
            pool.submit(runnable);
            pool.awaitQuiescence(3, TimeUnit.DAYS);
            pool.shutdown();
            pool.awaitTermination(30, TimeUnit.SECONDS);
            
            //Set up out.txt
            PrintWriter write = new PrintWriter("out.txt", "UTF-8");
            
            //Going through the maps and stuff to make the out.txt
            Set set = results.entrySet();
            Iterator iterate = set.iterator();
            
            while(iterate.hasNext()){
				//Going through each url that contains that word.
				Map.Entry mEntry = (Map.Entry)iterate.next();
                
                //Where we write to out.txt. Looks like --> ("WORD": ["URL", "URL", "URL"])
                write.println(mEntry.getKey() + ": " + mEntry.getValue());
            }
            write.close();
            
        }catch(Exception e){
            System.out.println("YOU DUN GOOFED AT URL STUFF: " + e);
        }        
    }
}
