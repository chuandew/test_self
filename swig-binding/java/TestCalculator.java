public class TestCalculator {
  static {
    try {
        System.loadLibrary("calculator"); // Load the generated C++ shared library
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" + e);
      System.exit(1);
    }
  }

    public static void main(String[] args) {
        Calculator calc = new Calculator();
        int result = calc.add(5, 3);
        System.out.println(result);  // Output: 8

        result = calc.subtract(8, 3);
        System.out.println(result);  // Output: 5
    }
}