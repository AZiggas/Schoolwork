

import java.net.URL;
import java.net.HttpURLConnection;
import java.net.*;
import java.util.*;
import java.lang.*;
import java.util.concurrent.ForkJoinPool;

//Jsoup located in org folder at root
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class Worker implements Runnable{
    URL url;
    HashSet<String> disallowed;
    HashSet<String> visited;
    TreeMap<String, ArrayList<String>> results;
    ForkJoinPool pool;
    int depth;

    Worker(URL url, HashSet<String> disallowed, HashSet<String> visited, TreeMap<String, ArrayList<String>> results, ForkJoinPool pool, int depth){
        this.url = url;
        this.disallowed = disallowed;
        this.visited = visited;
        this.results = results;
        this.pool = pool;
        this.depth = depth;
    }
	
    public void run(){
        //System.out.println("WEEWOO!");
        //We don't want a depth greater than 10 because it could possibly go on for a very VERY long time.
        if(depth < 10){
            
			try{
				Document d = Jsoup.connect(url.toString()).timeout(15000).get();
				String txt = d.text();
				Elements links = d.select("a[href]");
				//int code = 0; //The variable for the error code
					for(Element e : links){
						
						String href = e.attr("href");
						try{
							URL url2 = new URL(url, href); //Makes the URL absolute
								
							/*//This will be for checking the respons codes such as 404 and 200.
							HttpURLConnection urlCon = (HttpURLConnection)url2.openConnection();
							code = urlCon.getResponseCode();	
							
							if(code == 404){
								System.out.println("PAGE NOT FOUND!");
							}
							*/
							if(url2.getHost().equals(url.getHost()) && !disallowed.contains(url2.toString()) && !visited.contains(url2.toString())){
								
								//the gethost is to make sure we never leave the site. Can't have anything from disallowed list and already visited.
								visited.add(url2.toString());
								
								//Going throught the text taken from the site.
								Scanner tScanner = new Scanner(txt);
								
								while(tScanner.hasNext()){
									String word = tScanner.next().toLowerCase(); //To make sure there are no duplicates due to casing.
									
									
									if(results.containsKey(word)){ //We've already come across the word.
										ArrayList<String> list = results.get(word);
										list.add(url2.toString());
										results.replace(word, list);
									}
									else{
										ArrayList<String> list = new ArrayList<>();
										list.add(url2.toString());
										results.put(word, list);
									}
								}
								
								pool.submit(new Worker(url2, disallowed, visited, results, pool, depth+1));
							}
							
						}catch(MalformedURLException bad){
							System.out.println("ERROR WITH LINK: " + bad);
						}
						
					}
					
			}catch(Exception error){
				//if(code != 404){
					System.out.println("ERROR WITH PAGES: " + error);
				//}
			}
		}
    }
}
