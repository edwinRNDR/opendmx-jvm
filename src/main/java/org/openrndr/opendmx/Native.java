package org.openrndr.opendmx;

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

public class Native {

    static {
        // load native os specific libraries
        String os = System.getProperty("os.name").toLowerCase();
        if (os.contains("win")) {
            loadLibrary("/lib/org.openrndr.opendmx/windows-x64/ftd2xx64.dll", "ftd2xx64.dll");
            loadLibrary("/lib/org.openrndr.opendmx/windows-x64/native.dll", "native.dll");
        }
    }

    static void loadLibrary(String resource, String target) {
        InputStream stream = Native.class.getResourceAsStream(resource);

        try {
            File tmpDir = new File(System.getProperty("java.io.tmpdir"));
            File dmxDir = new File(tmpDir, "opendmx-jvm");
            dmxDir.mkdirs();

            File library = new File(dmxDir, target);

            if (true || !library.exists()) {
                OutputStream out = new FileOutputStream(library);
                byte[] buffer = new byte[1024];
                int len;
                while ((len = stream.read(buffer)) != -1) {
                    out.write(buffer, 0, len);
                }
                out.close();
            }
            System.load(library.getAbsolutePath());
        } catch (IOException e) {
            e.printStackTrace();
        }

    }


    public native static boolean connect(int _mode);

    public native static boolean disconnect();

    public native static void send(byte[] data );

}