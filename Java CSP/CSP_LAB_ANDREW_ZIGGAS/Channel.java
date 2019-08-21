import java.util.concurrent.*;
import java.util.concurrent.locks.*;

public abstract class Channel<T>{
    protected boolean closed=false;
    
    protected static Lock selectLock = new ReentrantLock();
    protected static Condition selectCondition = selectLock.newCondition();
    
    public static class SelectResult<X>{
        public Channel<X> channel;
        public X data;
        SelectResult(Channel<X> c, X d){
            channel=c;
            data=d;
        }
    }
        
    public void close(){
        synchronized(this){
            closed=true;
            notifyAll();
        }
    }
    
    public boolean isClosed(){
        synchronized(this){
            return closed;
        }
    }
    
    public abstract boolean put(T o);
    public abstract T get();
    public abstract T nonblockingGet();
    
    
    public static <X> SelectResult<X> select(Channel<X>... ch){
        selectLock.lock();
        try{
            while(true){
                for(Channel<X> c : ch ){
                    if( c.isClosed() )
                        throw new RuntimeException();
                    X tmp = c.nonblockingGet();
                    if( tmp != null )
                        return new SelectResult<X>( c,tmp );
                }
                try{
                    selectCondition.await();
                }
                catch(InterruptedException e){
                    System.exit(0);
                }
            }
        }
        finally{
            selectLock.unlock();
        }
    }
}
