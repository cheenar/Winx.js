public class Winx {
  // starts up the v8 engine, executes a string
  private native void startupV8(String filename);
  static {
    System.loadLibrary("winx");
  }

  public void helloJava() {
    System.out.println("Hello from java");
  }

  public static void main(final String[] args) {
    Winx winx = new Winx();
    winx.startupV8(args[0]);
  }
}
