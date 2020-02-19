/* ErrnoGenerator.java */
package recipeNo074;

// we need this one as we want to alter value of 
// the object inside JNI - java.lang.Integer is immutable
// this is why we have to use AtomicInteger
// Alternativelly, one can develop custom wrapper class
// that can hold and modify integer value
import java.util.concurrent.atomic.AtomicInteger;

public class ErrnoGenerator {

  // This method will call system function that will fail
  // In case of failure, errnoValue and errnoString will be set
  // to values reported by system call
  // shouldIFail == true  - system call will fail and set errnoValue and errnoString
  // shoudlIFail == false - system call will execute successfully and will not altee errnoValue and errnoString
  public native String generateErrno(boolean shouldIFail, AtomicInteger errnoValue, StringBuilder errnoString);

  static {
    System.loadLibrary("ErrnoGenerator");
  }

  public static void main(String[] args) {
    ErrnoGenerator gen = new ErrnoGenerator();

    boolean [] flags = { true, false };

    for( boolean flag : flags ) {

      StringBuilder errnoString = new StringBuilder("");
      AtomicInteger errnoValue  = new AtomicInteger(0);

      String result = gen.generateErrno( flag, errnoValue, errnoString );

      if( result == null ) {
        System.out.println("[J] " + (flag ? "true :" : "false:") + " result: null errno: " + errnoValue.get() + " message: " + errnoString.toString());
      } else {
        System.out.println("[J] " + (flag ? "true :" : "false:") + " result: " + result + " errno: everything is fine - don't check errno");
      }
    } 
 
  }
}
