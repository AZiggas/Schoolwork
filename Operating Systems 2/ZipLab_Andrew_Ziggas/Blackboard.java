
import java.util.LinkedList;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

public class Blackboard {
 
    static Map<String, LinkedList<Object[]> > board = new HashMap<>();
    
    /**
     * Get any tuple that matches any of the tags passed in
     * @param tags
     * @return The tuple or null if none found
     */
    public static Tuple nonblockingGet(String... tags){
        synchronized(board){
            for(String tag : tags ){
                if( board.containsKey(tag) ){
                    LinkedList<Object[]> A = board.get(tag);
                    Object[] tmp = A.remove();
                    if( A.isEmpty() )
                        board.remove(tag);
                    return new Tuple(tag,tmp);
                }
            }
            return null;
        }
    }
    
    public static Tuple nonblockingGet(Pattern... tags){
        synchronized(board){
            for(Pattern p : tags ){
                for( String tag : board.keySet() ){
                    if( p.matcher(tag).matches() ){
                        LinkedList<Object[]> A = board.get(tag);
                        Object[] tmp = A.remove();
                        if( A.isEmpty() )
                            board.remove(tag);
                        return new Tuple(tag,tmp);
                    }
                }
            }
            return null;
        }
    }
    
    public static Tuple get(String... tags){
        synchronized(board){
            Tuple x = nonblockingGet(tags);
            while( x == null ){
                try{
                    board.wait();
                }
                catch(InterruptedException e){
                    System.exit(1);
                }
                x = nonblockingGet(tags);
            }
            return x;
        }
    }
    
    public static Tuple getRegex(String... tags){
        Pattern[] p = new Pattern[tags.length];
        for(int i=0;i<tags.length;++i){
            p[i] = Pattern.compile(tags[i]);
        }
        return get(p);
    }
    
    public static Tuple get(Pattern... tags){
        synchronized(board){
            Tuple x = nonblockingGet(tags);
            while(x==null){
                try{
                    board.wait();
                }
                catch(InterruptedException e){
                    System.exit(1);
                }
                x = nonblockingGet(tags);
            }
            return x;
        }
    }
    
    
    public static void put(String tag, Object... data){
        synchronized(board){
            if( board.containsKey(tag))
                board.get(tag).add(data);
            else{
                LinkedList<Object[]> A = new LinkedList<>();
                A.add(data);
                board.put(tag,A);
            }
            board.notifyAll();
        }
    }
}