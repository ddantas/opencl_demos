int main(int argc, char** argv) {
  
  Iplimage img = ImRead(argv[1]);
  
  ImFlipH(img);
  ImWrite(img);
  
  return 0;
}