
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

import java.io.Serializable;

import java.util.List;
import java.util.ArrayList;

public class Test<Type>
		 implements Serializable {

	@Documented
	@Retention(RetentionPolicy.RUNTIME)
	public static @interface SomeAnnotation {

		String value() default "";

		Class<?> type() default Object.class;

	}

	private static final long serialUID = -1L;

	private Type o;

	@SomeAnnotation("Basic")
	public Test(@SomeAnnotation Type o) {
		this.o = o;
	}

	@Deprecated
	public Test(String o) 
			throws ClassNotFoundException,
			IllegalAccessException,
			InstantiationException {
		this.o = (Type)Class.forName(o).newInstance();
	}

	@SomeAnnotation(type = SomeAnnotation.class)
	public Test(Class<Type> t) 
			throws InstantiationException,
			IllegalAccessException {
		o = t.newInstance();
	}

	@Override
	public boolean equals(Object o) {
		if(!(o instanceof Test)) return false;
		return o.equals(((Test<Type>)o).o);
	}

}


