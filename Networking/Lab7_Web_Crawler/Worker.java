/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package lab7;


import java.net.URL;
import java.util.*;
import java.lang.*;
import java.util.concurrent.ExecutorService;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;


public class Worker implements Runnable{
    URL url;
    HashSet<String> disallowed;
    HashSet<String> visited;
    TreeMap<String, ArrayList<String>> results;
    ExecutorService pool;
    int depth;

    Worker(URL url, HashSet<String> disallowed, HashSet<String> visited, TreeMap<String, ArrayList<String>> results, ExecutorService pool, int depth){
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
        if(depth < 2){
            
			//int code = 0;
			try{
				Document d = Jsoup.connect(url.toString()).get();
				String txt = d.text();
				Elements links = d.select("a[href]");
				for(Element e : links){
					String href = e.attr("href");
					URL url2 = new URL(url, href); //Makes the URL absolute
					//HttpURLConnection urlCon = (HttpURLConnection)url2.openConnection();
					//code = urlCon.getResponseCode();	
					/*
					if(code == 404){
						System.out.println("PAGE NOT FOUND!");
					}
					*/
					if(url2.getHost().equals(url.getHost()) && !disallowed.contains(url2.toString()) && !visited.contains(url2.toString())){
						//Make a new worker and put into pool.
						visited.add(url2.toString());
						Scanner textScanner = new Scanner(txt);
						while(textScanner.hasNext()){
							String word = textScanner.next().toLowerCase(); //To make sure there are no duplicates due to casing.
							
							if(results.containsKey(word)){
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
						//System.out.println(url2.toString());
						//System.out.println(txt);
					}
					
				}
					
			}catch(Exception error){
				//if(code != 404){
					//System.out.println("ERROR WITH PAGES: " + error);
				// }
			}
		}
    }
}
