OBJDIR = bin/objs
graphics = $(addsuffix .o, $(addprefix $(OBJDIR)/, BMFont Camera Shader SpriteBatch Texture TextureAtlas))
lists = $(addsuffix .o, $(addprefix $(OBJDIR)/, BulletList EnemyList NodeList Vector2List))
math = $(addsuffix .o, $(addprefix $(OBJDIR)/, AStar Circle Interval Matrix4 Random Rectangle Vector2 Vector3 Vector4 VectorFuncs))
utils = $(addsuffix .o, $(addprefix $(OBJDIR)/, FileMethods IntStr MuleArray Parsing))
root = $(addsuffix .o, $(addprefix $(OBJDIR)/, Bullet Enemy EnemySpawner Game GameFunction Input Map_parsing Map Program Tower))
OBJS = $(graphics) $(lists) $(math) $(utils) $(root)

debug: $(OBJS)
	gcc -Wall $(OBJS) -o bin/Program.exe -L lib/ -mwindows -mconsole -lgdi32 -lglu32 -lglfw3 -lglew32 -lopengl32

run: debug
	bin/Program.exe

release: $(OBJS) resources
	gcc -Wall $(OBJS) -o bin/Program.exe -L lib/ -mwindows -lgdi32 -lglu32 -lglfw3 -lglew32 -lopengl32

clean:
	del /S /Q "bin\*"

$(graphics): $(OBJDIR)/%.o : src/Graphics/%.c | $(OBJDIR)
	gcc -Wall -c $< -o $@ -I include/ -D GLEW_STATIC

$(lists): $(OBJDIR)/%.o : src/Lists/%.c | $(OBJDIR)
	gcc -Wall -c $< -o $@ -I include/ -D GLEW_STATIC

$(math): $(OBJDIR)/%.o : src/Math/%.c | $(OBJDIR)
	gcc -Wall -c $< -o $@ -I include/ -D GLEW_STATIC

$(utils): $(OBJDIR)/%.o : src/Utils/%.c | $(OBJDIR)
	gcc -Wall -c $< -o $@ -I include/ -D GLEW_STATIC

$(root): $(OBJDIR)/%.o : src/%.c | $(OBJDIR)
	gcc -Wall -c $< -o $@ -I include/ -D GLEW_STATIC

$(OBJDIR):
	mkdir "$(OBJDIR)"

resources:
	xcopy /s /y "res" "bin\res\"