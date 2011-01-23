#include "myjson.h"
#include <cassert>

namespace Json {


// Class MyStyledWriter
// //////////////////////////////////////////////////////////////////

MyStyledWriter::MyStyledWriter()
   : rightMargin_( 74 )
   , indentSize_( 3 )
{
}


std::string 
MyStyledWriter::write( const Value &root )
{
   document_ = "";
   addChildValues_ = false;
   indentString_ = "";
   writeCommentBeforeValue( root );
   writeValue( root );
   writeCommentAfterValueOnSameLine( root );
   document_ += "\n";
   return document_;
}


void 
MyStyledWriter::writeValue( const Value &value ) {
   switch ( value.type() )
   {
   case nullValue:
	  pushValue( "null" );
	  break;
   case intValue:
	  pushValue( valueToString( value.asInt() ) );
	  break;
   case uintValue:
	  pushValue( valueToString( value.asUInt() ) );
	  break;
   case realValue:
	  pushValue( valueToString( value.asDouble() ) );
	  break;
   case stringValue:
	  pushValue( valueToQuotedString( value.asCString() ) );
	  break;
   case booleanValue:
	  pushValue( valueToString( value.asBool() ) );
	  break;
   case arrayValue:
	  writeArrayValue( value);
	  break;
   case objectValue:
	  {
		 Value::Members members( value.getMemberNames() );
		 if ( members.empty() )
			pushValue( "{}" );
		 else
		 {
		   // writeWithIndent( "{" );
			document_ += "{";
			//indent();
			Value::Members::iterator it = members.begin();
			while ( true )
			{
			   const std::string &name = *it;
			   const Value &childValue = value[name];
			   writeCommentBeforeValue( childValue );
			   //writeWithIndent( valueToQuotedString( name.c_str() ) );
			   document_ += valueToQuotedString( name.c_str() );
			   document_ += " : ";
			   writeValue( childValue );
			   if ( ++it == members.end() )
			   {
				  writeCommentAfterValueOnSameLine( childValue );
				  break;
			   }
			   document_ += ",";
			   writeCommentAfterValueOnSameLine( childValue );
			}
			//unindent();
			//writeWithIndent( "}" );
			document_ += "}";
		 }
	  }
	  break;
   }
}


void 
MyStyledWriter::writeArrayValue( const Value &value )
{
   unsigned size = value.size();
   if ( size == 0 )
      pushValue( "[]" );
   else
   {
      bool isArrayMultiLine = isMultineArray( value );
      if ( isArrayMultiLine )
      {
         writeWithIndent( "[" );
         indent();
         bool hasChildValue = !childValues_.empty();
         unsigned index =0;
         while ( true )
         {
            const Value &childValue = value[index];
            writeCommentBeforeValue( childValue );
            if ( hasChildValue )
               writeWithIndent( childValues_[index] );
            else
            {
               writeIndent();
               writeValue( childValue );
            }
            if ( ++index == size )
            {
               writeCommentAfterValueOnSameLine( childValue );
               break;
            }
            document_ += ",";
            writeCommentAfterValueOnSameLine( childValue );
         }
         unindent();
         writeWithIndent( "]" );
      }
      else // output on a single line
      {
         assert( childValues_.size() == size );
         document_ += "[ ";
         for ( unsigned index =0; index < size; ++index )
         {
            if ( index > 0 )
               document_ += ", ";
            document_ += childValues_[index];
         }
         document_ += " ]";
      }
   }
}


bool 
MyStyledWriter::isMultineArray( const Value &value )
{
   int size = value.size();
   bool isMultiLine = size*3 >= rightMargin_ ;
   childValues_.clear();
   for ( int index =0; index < size  &&  !isMultiLine; ++index )
   {
      const Value &childValue = value[index];
      isMultiLine = isMultiLine  ||
                     ( (childValue.isArray()  ||  childValue.isObject())  &&  
                        childValue.size() > 0 );
   }
   if ( !isMultiLine ) // check if line length > max line length
   {
      childValues_.reserve( size );
      addChildValues_ = true;
      int lineLength = 4 + (size-1)*2; // '[ ' + ', '*n + ' ]'
      for ( int index =0; index < size  &&  !isMultiLine; ++index )
      {
         writeValue( value[index] );
         lineLength += int( childValues_[index].length() );
         isMultiLine = isMultiLine  &&  hasCommentForValue( value[index] );
      }
      addChildValues_ = false;
      isMultiLine = isMultiLine  ||  lineLength >= rightMargin_;
   }
   return isMultiLine;
}


void 
MyStyledWriter::pushValue( const std::string &value )
{
   if ( addChildValues_ )
      childValues_.push_back( value );
   else
      document_ += value;
}


void 
MyStyledWriter::writeIndent()
{
   if ( !document_.empty() )
   {
      char last = document_[document_.length()-1];
      if ( last == ' ' )     // already indented
         return;
      if ( last != '\n' )    // Comments may add new-line
         document_ += '\n';
   }
   document_ += indentString_;
}


void 
MyStyledWriter::writeWithIndent( const std::string &value )
{
   writeIndent();
   document_ += value;
}


void 
MyStyledWriter::indent()
{
   indentString_ += std::string( indentSize_, ' ' );
}


void 
MyStyledWriter::unindent()
{
   assert( int(indentString_.size()) >= indentSize_ );
   indentString_.resize( indentString_.size() - indentSize_ );
}


void 
MyStyledWriter::writeCommentBeforeValue( const Value &root )
{
   if ( !root.hasComment( commentBefore ) )
      return;
   document_ += normalizeEOL( root.getComment( commentBefore ) );
   document_ += "\n";
}


void 
MyStyledWriter::writeCommentAfterValueOnSameLine( const Value &root )
{
   if ( root.hasComment( commentAfterOnSameLine ) )
      document_ += " " + normalizeEOL( root.getComment( commentAfterOnSameLine ) );

   if ( root.hasComment( commentAfter ) )
   {
      document_ += "\n";
      document_ += normalizeEOL( root.getComment( commentAfter ) );
      document_ += "\n";
   }
}


bool 
MyStyledWriter::hasCommentForValue( const Value &value )
{
   return value.hasComment( commentBefore )
          ||  value.hasComment( commentAfterOnSameLine )
          ||  value.hasComment( commentAfter );
}


std::string 
MyStyledWriter::normalizeEOL( const std::string &text )
{
   std::string normalized;
   normalized.reserve( text.length() );
   const char *begin = text.c_str();
   const char *end = begin + text.length();
   const char *current = begin;
   while ( current != end )
   {
      char c = *current++;
      if ( c == '\r' ) // mac or dos EOL
      {
         if ( *current == '\n' ) // convert dos EOL
            ++current;
         normalized += '\n';
      }
      else // handle unix EOL & other char
         normalized += c;
   }
   return normalized;
}

}
