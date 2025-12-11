import java.io.*;
import java.util.*;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

class ImgProc{
    private static int collapse_color(int argb){
        final int red = (argb & 0xF80000) >>> 8;
        final int green = (argb & 0xFC00) >>> 5;
        final int blue = (argb & 0xF8) >>> 3;
        return red | green | blue;
    }

    private static int reduce_color(int argb){
        return (argb & 0xF8FCF8) | 0xFF000000;
    }

    public static void main(String[] args) throws Exception {
        BufferedImage[] images = new BufferedImage[args.length];
        String[] names = new String[args.length];
        for(int i=0; i<args.length; i++){
            File f = new File(args[i]);
            names[i] = f.getName().split("[.]")[0].strip();
            BufferedImage img = ImageIO.read(f);
            if(img.getWidth() > 255){
                throw new AssertionError("Image \"" + args[i] + "\" is too wide.");
            }
            if(img.getHeight() > 255){
                throw new AssertionError("Image \"" + args[i] + "\" is too tall.");
            }
            images[i] = img;
        }
        System.out.println("typedef struct {");
        System.out.println("    const uint8_t width;");
        System.out.println("    const uint8_t height;");
        System.out.println("    const eadk_color_t back_color;");
        System.out.println("    const eadk_color_t* pixels;");
        System.out.println("} sprite_t;");
        System.out.println();
        HexFormat hf = HexFormat.of().withUpperCase();
        for(int i=0; i<args.length; i++){
            if(i != 0) System.out.println();
            BufferedImage img = images[i];
            String name = names[i];
            String data_name = name + "_data";
            HashSet<Integer> color_set = new HashSet<>();
            for(int y=0; y<img.getHeight(); y++){
                for(int x=0; x<img.getWidth(); x++){
                    final int color = img.getRGB(x, y);
                    final int alpha = color >>> 24;
                    if(alpha == 0xFF) color_set.add(collapse_color(color));
                }
            }
            int back_color = 0xFFFFFF;
            while(color_set.contains(collapse_color(back_color))){
                back_color -= 8;
            }
            String back_color_str = "0x" + hf.toHexDigits(collapse_color(back_color)).substring(4);
            System.out.print("const eadk_color_t " + data_name + "[] = {");
            for(int y=0; y<img.getHeight(); y++){
                for(int x=0; x<img.getWidth(); x++){
                    if((y != 0) || (x != 0)){
                        System.out.print(", ");
                    }
                    final int color = img.getRGB(x, y);
                    final int alpha = color >>> 24;
                    if(alpha == 0xFF){
                        System.out.print("0x" + hf.toHexDigits(collapse_color(img.getRGB(x, y))).substring(4));
                    } else {
                        System.out.print(back_color_str);
                    }
                }
            }
            System.out.println("};");
            System.out.println();
            System.out.println("const sprite_t " + name + " = {" + img.getWidth() + ", " + img.getHeight() + ", " + back_color_str + ", " + data_name + "};");
        }
    }
}