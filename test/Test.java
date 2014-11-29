
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import java.io.Serializable;

import java.util.List;
import java.util.ArrayList;

public class Test<Type, Type2 extends Number>
	implements Serializable, Cloneable {

	@MoreAnnotations({
		@SomeAnnotation("A String value"),
		@SomeAnnotation(clazz = String.class)
	})
	public String string = "A String";
	public final String finalString = "A final String";

	@AnotherAnnotation(count = 100)
	public static String staticFinalString = "A final and static String.";

	@SomeAnnotation(clazz = Integer.class)
	protected final Integer autoboxedInteger = 12345;
	protected static Double autoboxedDouble = 0xABC.DEFp+5D;

	@SomeAnnotation("A different value")
	final char[] finalCharArray = { 'a', 'b', 'c', 'd', 'e' };
	String[][] finalArrayOfStringArrays = {
		{ "The", "first", "String", "array" },
		{ "The", "second", "String", "array" }
	};

	@SomeAnnotation
	@AnotherAnnotation(count = 0)
	private Object anObject = new Object();
	private static Object instance = new Test<String, Short>();

	@MoreAnnotations(
		value = { },
		type = SomeEnum.TWO
	)
	private static List<String> stringList = new ArrayList<>();

	static {
		stringList.add("1");
		stringList.add("2");
		stringList.add("3");
	}

	@AnotherAnnotation(count = 1000)
	public static enum SomeEnum {
		ONE, TWO, THREE;
	}

	@Documented
	@Retention(RetentionPolicy.RUNTIME)
	protected static @interface SomeAnnotation {
		String value() default "A Value";
		Class<?> clazz() default void.class;
	}

	@Documented
	@Retention(RetentionPolicy.RUNTIME)
	private static @interface MoreAnnotations {
		SomeAnnotation[] value();
		SomeEnum type() default SomeEnum.ONE;
	}

	@Documented
	@Retention(RetentionPolicy.CLASS)
	public static @interface AnotherAnnotation {
		int count();
	}

	public abstract class Foo {
		public abstract void foo();
	}

	public class Bar extends Foo {
		public void foo() {
			System.out.println("foo!");
		}
	}

	public Test() {
	}

	public Test(Object object) {
		this.anObject = object;
	}

	public Test(String name) {
		this.string = name;
	}

	public int foo() {
		return string.hashCode() + autoboxedDouble.toString().hashCode();
	}

	public native Type2 foo(int x);

	public static String bar() {
		return Test.bar(staticFinalString);
	}

	public static String bar(String seed) {
		StringBuilder builder = new StringBuilder(seed);
		for(String value : stringList)
			builder.append(value);
		return builder.toString();
	}

	protected final synchronized void bat() {
		Foo foo = new Bar();
		foo.foo();
	}

	void bat(Type value) throws RuntimeException {
		throw new RuntimeException(value.toString());
	}

	@Override
	public boolean equals(Object o) {
		if(!(o instanceof Test)) return false;

		Test t = (Test)o;
		return t.string.equals(string);
	}

}
