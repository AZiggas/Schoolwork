import java.util.*;
import java.util.concurrent.*;

public class BufferedChannel<T> extends Channel<T>{
    private LinkedList<T> Q;
    private int capacity;
    public BufferedChannel(int capacity){
        Q = new LinkedList<>();
        this.capacity = capacity;
    }
    
    public boolean put(T o){
        synchronized(this){
            if(isClosed())
                throw new RuntimeException();
            
            while(Q.size() >= capacity ){
                try{
                    wait();
                }
                catch(InterruptedException e){
                    System.exit(0);
                }
            }
            
            if(isClosed())
                throw new RuntimeException();
           
            Q.add(o);
            notifyAll();
        }
        selectLock.lock();
        selectCondition.signalAll();
        selectLock.unlock();
        return true;
    }
    
    public T get(){
        synchronized(this){
            while(Q.size() == 0 ){
                if( isClosed() )
                    return null;
                try{
                    wait();
                }
                catch(InterruptedException e){
                    System.exit(0);
                }
            }
            T x = Q.remove();
            notifyAll();
            return x;
        }
    }
    public T nonblockingGet(){
        synchronized(this){
            if( Q.size() > 0 ){
                T x = Q.remove();
                notifyAll();
                return x;
            }
            return null;
        }
    }
}
